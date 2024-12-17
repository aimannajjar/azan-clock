#include "wifi.h"
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "azan_clock.h"
#include "systime.h"

#define TAG "WiFi"
#define MAX_NETWORKS 20
#define MAX_OPTION_SIZE 33
#define NVS_NAMESPACE "wifi"

static char ssid[33];
static char password[64];
extern lv_obj_t *ui_WiFi_Networks;
extern lv_obj_t *ui_WiFi_Password;
extern lv_obj_t *ui_Main_Screen;
extern lv_obj_t *ui_Setup_Screen;
extern lv_obj_t *ui_Loading_Screen;
extern lv_obj_t *ui_Loading_Status_Text;
static lv_obj_t *modal_msgbox = NULL; // Message box object
extern SemaphoreHandle_t lvgl_mutex;
static TaskHandle_t wifi_scan_task_handle = NULL;
static bool is_scanning = false; // Add this global variable

static void save_connection_params(const char *ssid, const char *password);
static esp_err_t load_connection_params(char *ssid, size_t ssid_size, char *password, size_t password_size);
static void lock_and_wifi_setup_mode();
static void lock_and_close_message_box_cb(lv_timer_t *timer);
static void lock_and_show_message_box(const char *text);
static void connect_to_saved_wifi();
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void wifi_init() {
    take_ui_mutex("wifi_init");
    lv_label_set_text(ui_Loading_Status_Text, "Connecting to Wi-Fi...");
    give_ui_mutex("wifi_init");

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

        // Save the parameters on successful connection
        save_connection_params(ssid, password);

        if (lv_scr_act() == ui_Setup_Screen) {
            lock_and_show_message_box("Connected");
            lv_timer_create(lock_and_close_message_box_cb, 2000, NULL); // Close after 2 seconds    
        }

        take_ui_mutex("wifi_event_handler");
        if (!is_wifi_previously_connected()) {
            // If system has not been initialized
            // show the loading screen, that's because we are still waiting for IP address
            lv_label_set_text(ui_Loading_Status_Text, "Requesting IP address...");
            lv_scr_load(ui_Loading_Screen);
            set_wifi_previously_connected();
        } else {
            // If system has already been initialized
            // TODO: Update the Wi-Fi status icon on the main screen
            lv_scr_load(ui_Main_Screen);
        }
        give_ui_mutex("wifi_event_handler");

        // Next event that will take place is IP address assignment
        // proceed tracing in the IP event handler branch below
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi connection failed.");

        if (lv_scr_act() != ui_Setup_Screen && !is_wifi_previously_connected()) {
            // If we are not on the setup screen already and system has not been initialized
            // this means we failed to connect during boot, so show the setup screen
            lock_and_wifi_setup_mode();
        } else if (lv_scr_act() == ui_Setup_Screen) {
            // if we are already on setup screen, show a message box
            lock_and_show_message_box("Failed to connect...");
            start_scan_task(NULL);
            lv_timer_create(lock_and_close_message_box_cb, 2000, NULL); // Close after 2 seconds    
        } else {
            // If system had already been initialized and we lost connection 
            // TODO: Update wi-ifi status icon on the main screen
        }
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        is_scanning = false; // Update scanning state
        ESP_LOGI(TAG, "Wi-Fi scan complete.");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // Whenever we get a new IP address, (re-)initialize the system time
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
        systime_init();
    }    
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
    // Disconnect from any existing connection first
    esp_wifi_disconnect();
    
    // This gets called when the "Connect" button is pressed
    // Get the selected Wi-Fi SSID from the dropdown
    lv_dropdown_get_selected_str(ui_WiFi_Networks, ssid, sizeof(ssid));

    // Get the password entered in the textarea
    const char *password_text = lv_textarea_get_text(ui_WiFi_Password);
    strncpy(password, password_text, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0'; // Ensure null termination

    // Stop scanning before configuring new connection
    stop_scan_task(NULL);

    // Wait for any ongoing scan operation to complete
    while (is_scanning) {
        ESP_LOGI(TAG, "Waiting for scan operation to complete...");
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // Configure Wi-Fi connection
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_LOGI(TAG, "Connecting to SSID: %s", ssid);
    lock_and_show_message_box("Connecting...");

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
}


// ---------------------------------------------------
// Functions to scan Wi-Fi networks
// ---------------------------------------------------
void start_scan_task(lv_event_t *e) {
    if (wifi_scan_task_handle != NULL)
        return;

    xTaskCreate(wifi_scan_task, "wifi_scan_task", 8192, NULL, 5, &wifi_scan_task_handle);
}

void stop_scan_task(lv_event_t *e) {
    if (wifi_scan_task_handle != NULL) {
        ESP_LOGI(TAG, "Stopping Wi-Fi scan task...");
        xTaskNotifyGive(wifi_scan_task_handle);
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

        // Wait for either timeout or notification
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(10000))) {
            // If we received a notification, exit the task
            break;
        }
    }
    wifi_scan_task_handle = NULL;
    vTaskDelete(NULL);
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

// ----------------------------------------
// Helper Functions to show/hide message box
// ----------------------------------------
static void lock_and_show_message_box(const char *text) {
    if (lv_scr_act() != ui_Setup_Screen)
        return;
    take_ui_mutex("lock_and_show_message_box");
    if (!modal_msgbox) {
        modal_msgbox = lv_msgbox_create(NULL, text, text, NULL, false);
        lv_obj_align(modal_msgbox, LV_ALIGN_CENTER, 0, 0);
    } else {
        // Locate the internal label and update its text
        lv_obj_t *label = lv_msgbox_get_text(modal_msgbox);
        lv_label_set_text(label, text);
    }
    lv_obj_set_style_bg_color(modal_msgbox, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
    give_ui_mutex("lock_and_show_message_box");
}

static void lock_and_close_message_box_cb(lv_timer_t *timer) {
    take_ui_mutex("lock_and_close_message_box_cb");
    if (modal_msgbox) {
        lv_obj_del(lv_obj_get_parent(modal_msgbox));
        modal_msgbox = NULL;
    }
    give_ui_mutex("lock_and_close_message_box_cb");
    lv_timer_del(timer);
}
