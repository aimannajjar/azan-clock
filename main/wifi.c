#include "wifi.h"
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "azan_clock.h"
#include "systime.h"
#include "freertos/semphr.h"
#include "ui/components/ui_comp_leftpanel.h"
#include <sys/time.h>

#define TAG "WiFi"
#define MAX_NETWORKS 20
#define MAX_OPTION_SIZE 33
#define NVS_NAMESPACE "wifi"

#define MAX_RETRIES 3

extern lv_obj_t *ui_Left_Panel_Main;
extern lv_obj_t *ui_Left_Panel_Prayers;
extern lv_obj_t *ui_Left_Panel_Setup;
extern lv_obj_t *ui_Left_Panel_Settings;
extern lv_obj_t *ui_Connecting_Modal;
extern lv_obj_t *ui_Connection_Failed_Label;

static int current_retries = 0;
static char ssid[33];
static char password[64];
extern lv_obj_t *ui_WiFi_Networks;
extern lv_obj_t *ui_WiFi_Password;
extern lv_obj_t *ui_Main_Screen;
extern lv_obj_t *ui_Setup_Screen;
extern lv_obj_t *ui_Loading_Screen;
extern lv_obj_t *ui_Loading_Status_Text;
static TaskHandle_t wifi_scan_task_handle = NULL;
static TaskHandle_t retry_saved_connection_handle = NULL;
static SemaphoreHandle_t wifi_mux = NULL;
static bool is_scanning = false;

static void save_connection_params(const char *ssid, const char *password);
static esp_err_t load_connection_params(char *ssid, size_t ssid_size, char *password, size_t password_size);
static void lock_and_wifi_setup_mode();
static void connect_to_saved_wifi();
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void show_disconnected_icon();
static void hide_disconnected_icon();
static void take_wifi_semaphore(const char *caller);
static void give_wifi_semaphore(const char *caller);

void wifi_init() {
    take_ui_mutex("wifi_init");
    lv_label_set_text(ui_Loading_Status_Text, "Connecting to Wi-Fi...");
    give_ui_mutex("wifi_init");

    wifi_mux = xSemaphoreCreateBinary();
    give_wifi_semaphore("wifi_init -> Initial Release");

    // Initialize WiFI stack
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Register Wi-Fi and IP event handlers (only once)
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    // Load saved Wi-Fi parameters and connect 
    // or show setup screen if not previously saved
    if (load_connection_params(ssid, sizeof(ssid), password, sizeof(password)) == ESP_OK) {
        connect_to_saved_wifi();
    } else {
        // First time setup, show the Wi-Fi setup screen
        ESP_LOGI(TAG, "No saved Wi-Fi parameters found. Show Wi-Fi setup screen.");
        lock_and_wifi_setup_mode();
    }
}

// Event Handler for Wi-Fi and IP events
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // scanning or connecting
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        // Connected to the AP!
        ESP_LOGI(TAG, "Wi-Fi connected to the network.");
        current_retries = 0; // Reset retry counter

        // Save the parameters on successful connection
        save_connection_params(ssid, password);

        if (!is_wifi_initialized()) {
            // We are going through boot init sequence
            set_wifi_initialized();
            take_ui_mutex("wifi_event_handler");
            lv_label_set_text(ui_Loading_Status_Text, "Requesting IP address...");
            lv_obj_add_flag(ui_Connecting_Modal, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Connection_Failed_Label, LV_OBJ_FLAG_HIDDEN);
            give_ui_mutex("wifi_event_handler");
            // Next event that will take place is IP address assignment
            // proceed tracing in the IP event handler branch below
        } else {
            // We are either reconnecting or new wi-fi network was manually chosen
            // If it was manual Wi-Fi setup, hide spinner and show the main screen on successful connection
            if (lv_scr_act() == ui_Setup_Screen) {
                take_ui_mutex("wifi_event_handler");
                lv_obj_add_flag(ui_Connecting_Modal, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_Connection_Failed_Label, LV_OBJ_FLAG_HIDDEN);
                lv_scr_load(ui_Main_Screen);
                give_ui_mutex("wifi_event_handler");
            } else {
                take_ui_mutex("wifi_event_handler");
                hide_disconnected_icon();
                give_ui_mutex("wifi_event_handler");
            }
        }
        give_wifi_semaphore("wifi_event_handler -> WIFI_EVENT_STA_CONNECTED");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGE(TAG, "Wi-Fi connection disconnected.");
        give_wifi_semaphore("wifi_event_handler -> WIFI_EVENT_STA_DISCONNECTED");
        if (lv_scr_act() != ui_Setup_Screen && !is_wifi_initialized()) {
            // If we are not on the setup screen already and system has not been initialized
            // this means we failed to connect during boot, so show the setup screen after exhausting retries
            if (current_retries < MAX_RETRIES) {
                current_retries++;
                ESP_LOGI(TAG, "Retry #%d to connect to Wi-Fi...", current_retries);
                connect_to_saved_wifi();
            } else {
                // Max retries reached, show the setup screen
                lock_and_wifi_setup_mode();
            }
        } else if (lv_scr_act() == ui_Setup_Screen) {
            // Connection failed during setup screen
            take_ui_mutex("wifi_event_handler");
            lv_obj_add_flag(ui_Connecting_Modal, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_Connection_Failed_Label, LV_OBJ_FLAG_HIDDEN);

            give_ui_mutex("wifi_event_handler");
            start_scan_task(NULL);
        } else {
            // We have lost Wi-Fi connection after having been fully initialized before
            take_ui_mutex("wifi_event_handler");
            show_disconnected_icon();
            give_ui_mutex("wifi_event_handler");            
            if (retry_saved_connection_handle == NULL) {
                xTaskCreate(wifi_scan_task, "retry_saved_connection", 4096, NULL, 5, &retry_saved_connection_handle);
            }
        }
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        is_scanning = false; // Update scanning state
        ESP_LOGI(TAG, "Wi-Fi scan complete.");
        // Lock is released in wifi_scan_task
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // Whenever we get a new IP address, (re-)initialize the system time
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP Address: " IPSTR, IP2STR(&event->ip_info.ip));

        if (!is_systime_initialized()) {
            systime_init();
        } else {
            notify_systime();
        }
    }    
}

// Helper function to set/clear disconnected wi-fi icon
static void show_disconnected_icon() {
    ESP_LOGI(TAG, "Showing disconnected icon...");
    lv_obj_clear_flag(ui_comp_get_child(ui_Left_Panel_Main, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_comp_get_child(ui_Left_Panel_Prayers, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_comp_get_child(ui_Left_Panel_Settings, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_comp_get_child(ui_Left_Panel_Setup, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
}

static void hide_disconnected_icon() {
    ESP_LOGI(TAG, "Hiding disconnected icon...");
    lv_obj_add_flag(ui_comp_get_child(ui_Left_Panel_Main, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_comp_get_child(ui_Left_Panel_Prayers, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_comp_get_child(ui_Left_Panel_Settings, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_comp_get_child(ui_Left_Panel_Setup, UI_COMP_LEFTPANEL_WIFI_DISCONNECTED), LV_OBJ_FLAG_HIDDEN);
}

static void take_wifi_semaphore(const char *caller) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ESP_LOGE(TAG, "[%lld.%06ld] Wi-Fi Semaphore: Taking from %s", (long long)tv.tv_sec, (long)tv.tv_usec, caller);
    xSemaphoreTake(wifi_mux, portMAX_DELAY);
}

static void give_wifi_semaphore(const char *caller) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ESP_LOGE(TAG, "[%lld.%06ld] Wi-Fi Semaphore: Giving from %s", (long long)tv.tv_sec, (long)tv.tv_usec, caller);
    xSemaphoreGive(wifi_mux);
}

// ---------------------------------------------------
// Helper Function to connect to saved Wi-Fi
// ---------------------------------------------------
static void connect_to_saved_wifi() {
    // Saved Wi-Fi parameters found, use them to connect
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_LOGI(TAG, "Using saved Wi-Fi parameters to connect...");
    take_wifi_semaphore("connect_to_saved_wifi");
    ESP_LOGI(TAG, "Start connection...");
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

// ---------------------------------------------------
// Wi-Fi Setup Mode functions
// ---------------------------------------------------
// Helper function to show the Wi-Fi setup screen
static void lock_and_wifi_setup_mode() {
    take_ui_mutex("lock_and_wifi_setup_mode");
    lv_scr_load(ui_Setup_Screen);
    give_ui_mutex("lock_and_wifi_setup_mode");
    ESP_ERROR_CHECK(esp_wifi_start());
    start_scan_task(NULL);
}

// Connect button callback
void connect_wifi(lv_event_t *e) {
    // Show connecting spinner and hide previous failure message if any
    lv_obj_clear_flag(ui_Connecting_Modal, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Connection_Failed_Label, LV_OBJ_FLAG_HIDDEN);

    // This gets called when the "Connect" button is pressed
    // Get the selected Wi-Fi SSID from the dropdown
    lv_dropdown_get_selected_str(ui_WiFi_Networks, ssid, sizeof(ssid));

    // Get the password entered in the textarea
    const char *password_text = lv_textarea_get_text(ui_WiFi_Password);
    strncpy(password, password_text, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0'; // Ensure null termination

    // Stop scanning before configuring new connection
    stop_scan_task(NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Wait for any ongoing scan operation to complete
    while (is_scanning) {
        ESP_LOGI(TAG, "Waiting for scan operation to complete...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    take_wifi_semaphore("connect_wifi -> connect");
    // Configure Wi-Fi connection
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_LOGI(TAG, "Connecting to SSID: %s", ssid);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
}

// Task used to initiate retry
void retry_saved_connection(void *param) {
    // Wait for either timeout or notification
    while (true)
    {
        ESP_LOGI(TAG, "Retry saved Wi-Fi connection...");
        connect_to_saved_wifi();
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(10000))) {
            // If we received a notification, exit the task
            break;
        }
    }
    vTaskDelete(retry_saved_connection_handle);
}

// ---------------------------------------------------
// Functions to scan Wi-Fi networks
// ---------------------------------------------------
void start_scan_task(lv_event_t *e) {
    ESP_LOGI(TAG, "Starting Wi-Fi scan task...");
    if (wifi_scan_task_handle != NULL) {
        ESP_LOGE(TAG, "Wi-Fi task was already started...");
        return;
    }

    if (retry_saved_connection_handle != NULL) {
        xTaskNotifyGive(retry_saved_connection_handle);
    }
    xTaskCreate(wifi_scan_task, "wifi_scan_task", 8192, NULL, 5, &wifi_scan_task_handle);
}

void stop_scan_task(lv_event_t *e) {
    if (wifi_scan_task_handle != NULL) {
        ESP_LOGI(TAG, "Stopping Wi-Fi scan task...");
        xTaskNotifyGive(wifi_scan_task_handle);
    } else {
        ESP_LOGE(TAG, "Wi-Fi task was already stopped...");
    }
}

char *generate_wifi_list() {
    uint16_t num_networks = MAX_NETWORKS;
    wifi_ap_record_t *ap_records = malloc(MAX_NETWORKS * sizeof(wifi_ap_record_t));
    if (!ap_records) {
        ESP_LOGE(TAG, "Failed to allocate memory for Wi-Fi records");
        return NULL;
    }

    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&num_networks, ap_records));

    char *dropdown_options = malloc(num_networks * MAX_OPTION_SIZE);
    if (!dropdown_options) {
        ESP_LOGE(TAG, "Failed to allocate memory for dropdown options");
        free(ap_records);
        return NULL;
    }

    dropdown_options[0] = '\0';

    for (int i = 0; i < num_networks; i++) {
        strcat(dropdown_options, (char *)ap_records[i].ssid);
        if (i < num_networks - 1) {
            strcat(dropdown_options, "\n");
        }
        ESP_LOGI(TAG, "SSID: %s, RSSI: %d", ap_records[i].ssid, ap_records[i].rssi);
    }

    free(ap_records);
    return dropdown_options;
}

void wifi_scan_task(void *param) {
    while (true) {
        take_wifi_semaphore("wifi_scan_task");
        wifi_scan_config_t scan_config = {
            .ssid = NULL,
            .bssid = NULL,
            .channel = 0,
            .show_hidden = false
        };

        is_scanning = true; // Update scanning state
        ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));

        char *dropdown_options = generate_wifi_list();
        if (dropdown_options) {
            take_ui_mutex("wifi_scan_task");
            lv_dropdown_set_options(ui_WiFi_Networks, dropdown_options);
            give_ui_mutex("wifi_scan_task");
            free(dropdown_options);
        }
        give_wifi_semaphore("wifi_scan_task");

        // Wait for either timeout or notification
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(10000))) {
            // If we received a notification, exit the task
            break;
        }
    }
    vTaskDelete(wifi_scan_task_handle);
    wifi_scan_task_handle = NULL;

}

// ---------------------------------------------------
// Helper Functions to read connection params from NVS
// ---------------------------------------------------
// Save connection parameters to NVS
static void save_connection_params(const char *ssid, const char *password) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS: %s", esp_err_to_name(err));
        return;
    }

    // Write SSID and password
    nvs_set_str(nvs_handle, "ssid", ssid);
    nvs_set_str(nvs_handle, "password", password);

    // Commit changes
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

    ESP_LOGI(TAG, "Connection parameters saved to NVS (%s, %s)", ssid, password);
}

// Load connection parameters from NVS
esp_err_t load_connection_params(char *ssid, size_t ssid_size, char *password, size_t password_size) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "No saved Wi-Fi parameters in NVS");
        return ESP_ERR_NVS_NOT_FOUND;
    }

    // Read SSID and password
    err = nvs_get_str(nvs_handle, "ssid", ssid, &ssid_size);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    } else if (ssid[0] == '\0') {
        ESP_LOGW(TAG, "SSID is empty");
        return ESP_ERR_NOT_FOUND;
    }

    err = nvs_get_str(nvs_handle, "password", password, &password_size);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        return err;
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "Loaded Wi-Fi parameters from NVS: SSID=%s", ssid);
    return ESP_OK;
}
