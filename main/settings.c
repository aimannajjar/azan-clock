// ...existing code...

#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_log.h"
#include "cJSON.h"
#include "azan_clock.h"
#include "lvgl/lvgl.h"

#define TAG "Settings"

extern lv_obj_t *ui_Latitude;
extern lv_obj_t *ui_Longitude;
extern lv_obj_t *ui_Location_Name;

// Function to get the user's location using ipstack.com API
void get_user_location(lv_event_t *e) {
    char url[256];
    const char *api_key = "22862314071ccda94c8a4f3c5384e28f"; // Replace with your actual API key

    snprintf(url, sizeof(url), "http://api.ipstack.com/check?access_key=%s", api_key);

    esp_http_client_config_t config = {
        .url = url,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int content_length = esp_http_client_get_content_length(client);
        ESP_LOGI(TAG, "Content Length: %d", content_length);
        char *buffer = malloc(content_length + 1);
        if (buffer) {
            int read_len = esp_http_client_read(client, buffer, content_length);
            if (read_len >= 0) {
                buffer[read_len] = '\0';
                cJSON *json = cJSON_Parse(buffer);
                if (json) {
                    cJSON *lat = cJSON_GetObjectItem(json, "latitude");
                    cJSON *lon = cJSON_GetObjectItem(json, "longitude");
                    cJSON *city = cJSON_GetObjectItem(json, "city");
                    if (lat && lon) {
                        ESP_LOGI(TAG, "User location: Latitude = %f, Longitude = %f, City = %s", lat->valuedouble, lon->valuedouble, city->valuestring);
                        
                        take_ui_mutex("get_user_location");
                        lv_textarea_set_text(ui_Latitude, lat->valuestring);
                        lv_textarea_set_text(ui_Longitude, lon->valuestring);
                        lv_label_set_text(ui_Location_Name, city->valuestring);
                        give_ui_mutex("get_user_location");
                    }
                    cJSON_Delete(json);
                } else {
                    ESP_LOGE(TAG, "Failed to parse JSON response");
                }
            } else {
                ESP_LOGE(TAG, "Failed to read response");
            }
            free(buffer);
        } else {
            ESP_LOGE(TAG, "Failed to allocate memory for response buffer");
        }
    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}