#include "wifi.h"
#include <string.h>
#include <stdlib.h>

#define TAG "WiFi"
#define MAX_NETWORKS 20
#define MAX_OPTION_SIZE 33

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Wi-Fi STA started, attempting to connect...");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi connected to the network.");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi disconnected. Retrying...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
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

void wifi_init() {
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

    // Start the Wi-Fi driver
    ESP_ERROR_CHECK(esp_wifi_start());
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
    char ssid[33];
    char password[64];

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

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
}
