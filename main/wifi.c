#include "wifi.h"
#include <string.h>
#include <stdlib.h>
#include "nvs.h"
#include "nvs_flash.h"

#define TAG "WiFi"
#define MAX_NETWORKS 20
#define MAX_OPTION_SIZE 33
#define NVS_NAMESPACE "wifi"

static lv_obj_t *modal_msgbox = NULL; // Message box object
static char ssid[33];
static char password[64];

static void save_connection_params(const char *ssid, const char *password);
static esp_err_t load_connection_params(char *ssid, size_t ssid_size, char *password, size_t password_size);

static void show_message_box(const char *text) {
    if (lv_scr_act() != ui_Setup_Screen)
        return;
    if (!modal_msgbox) {
        modal_msgbox = lv_msgbox_create(NULL, text, text, NULL, false);
        lv_obj_align(modal_msgbox, LV_ALIGN_CENTER, 0, 0);
    } else {
        // Locate the internal label and update its text
        lv_obj_t *label = lv_msgbox_get_text(modal_msgbox);
        lv_label_set_text(label, text);
    }
    lv_obj_set_style_bg_color(modal_msgbox, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN);
}

static void close_message_box_cb(lv_timer_t *timer) {
    if (modal_msgbox) {
        lv_obj_del(lv_obj_get_parent(modal_msgbox));
        modal_msgbox = NULL;
    }
    lv_timer_del(timer);
}

static void connected(lv_timer_t *timer) {
    if (modal_msgbox) {
        lv_obj_del(lv_obj_get_parent(modal_msgbox));
        modal_msgbox = NULL;
    }

    // Save the parameters on successful connection
    save_connection_params(ssid, password);

    lv_scr_load(ui_Main_Screen);
    lv_timer_del(timer);
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // scanning or connecting
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi connected to the network.");
        show_message_box("Connected");
        lv_timer_create(connected, 2000, NULL); // Close after 2 seconds
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi connection failed.");
        show_message_box("Failed to connect...");
        lv_timer_create(close_message_box_cb, 2000, NULL); // Close after 2 seconds
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

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

// returns true if previous connection params was restored from flash, false otherwise
bool wifi_init() {
    // Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create the default Wi-Fi station interface
    esp_netif_create_default_wifi_sta();

    // Initialize the Wi-Fi driver with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Set Wi-Fi mode to STA (station)
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Register Wi-Fi and IP event handlers (only once)
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));

    if (load_connection_params(ssid, sizeof(ssid), password, sizeof(password)) == ESP_OK) {
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
        return true;
    } else {
        ESP_LOGI(TAG, "No saved Wi-Fi parameters found. Waiting for user input.");
        ESP_ERROR_CHECK(esp_wifi_start());
        start_scan_task();
        return false;
    }
}

void start_scan_task() {
    ui_update_queue = xQueueCreate(5, sizeof(char *));
    if (!ui_update_queue) {
        ESP_LOGE(TAG, "Failed to create queue for UI updates");
        return;
    }

    // Create tasks
    xTaskCreate(wifi_scan_task, "wifi_scan_task", 8192, NULL, 5, NULL);
    xTaskCreate(lvgl_task, "lvgl_task", 4096, NULL, 5, NULL);
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
    while (1) {
        wifi_scan_config_t scan_config = {
            .ssid = NULL,
            .bssid = NULL,
            .channel = 0,
            .show_hidden = false
        };

        ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
        ESP_LOGI(TAG, "Wi-Fi scan complete");

        char *dropdown_options = generate_wifi_list();
        if (dropdown_options) {
            xQueueSend(ui_update_queue, &dropdown_options, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void lvgl_task(void *param) {
    char *dropdown_options;

    while (1) {
        if (xQueueReceive(ui_update_queue, &dropdown_options, portMAX_DELAY)) {
            lv_dropdown_set_options(ui_WiFi_Networks, dropdown_options);
            free(dropdown_options);
        }

        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void connect_wifi(lv_event_t *e) {
    // Get the selected Wi-Fi SSID from the dropdown
    lv_dropdown_get_selected_str(ui_WiFi_Networks, ssid, sizeof(ssid));

    // Get the password entered in the textarea
    const char *password_text = lv_textarea_get_text(ui_WiFi_Password);
    strncpy(password, password_text, sizeof(password) - 1);
    password[sizeof(password) - 1] = '\0'; // Ensure null termination

    // Configure Wi-Fi connection
    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_LOGI(TAG, "Connecting to SSID: %s", ssid);
    show_message_box("Connecting...");

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
}
