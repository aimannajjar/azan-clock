// ...existing code...

#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_log.h"
#include "cJSON.h"
#include "azan_clock.h"
#include "lvgl/lvgl.h"
#include "timezones.h"

#define TAG "Settings"

extern lv_obj_t *ui_Latitude;
extern lv_obj_t *ui_Longitude;
extern lv_obj_t *ui_Location_Name;
extern lv_obj_t *ui_Timezone_Dropdown;
extern lv_obj_t *ui_Keypad;

// Declare response buffer and length
static char location_response[1024];
static int response_len = 0;

// HTTP event handler to collect the response
static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {

    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            // Append received data to location_response buffer
            if (evt->data_len + response_len < sizeof(location_response)) {
                memcpy(location_response + response_len, evt->data, evt->data_len);
                response_len += evt->data_len;
                location_response[response_len] = '\0'; // Null-terminate
            } else {
                ESP_LOGW(TAG, "location_response buffer is full, cannot append more data");
                // Optionally handle buffer overflow
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            break;
    }
    return ESP_OK;

}

// Function to get the user's location using ipstack.com API
void get_user_location(lv_event_t *e) {
    // Reset response buffer
    response_len = 0;
    memset(location_response, 0, sizeof(location_response));

    char url[256];
    snprintf(url, sizeof(url), "https://locate.hurranet.net/prod/check");

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP request completed: Status = %d, Content-Length = %d, Content = %s",
                 (int)esp_http_client_get_status_code(client),
                 (int)esp_http_client_get_content_length(client),
                 location_response);

        cJSON *json = cJSON_Parse(location_response);
        if (json) {
            cJSON *lat = cJSON_GetObjectItem(json, "latitude");
            cJSON *lon = cJSON_GetObjectItem(json, "longitude");
            cJSON *city = cJSON_GetObjectItem(json, "city");
            cJSON *time_zone = cJSON_GetObjectItem(json, "time_zone");
            cJSON *time_zone_id = cJSON_GetObjectItem(time_zone, "id");
            if (lat && lon && city && time_zone_id) {
                ESP_LOGI(TAG, "User location: Latitude = %f, Longitude = %f, City = %s",
                         lat->valuedouble, lon->valuedouble, city->valuestring);

                char lat_str[16];
                char lon_str[16];
                snprintf(lat_str, sizeof(lat_str), "%f", lat->valuedouble);
                snprintf(lon_str, sizeof(lon_str), "%f", lon->valuedouble);

                ESP_LOGI(TAG, "Updating setting text fields");
                lv_textarea_set_text(ui_Latitude, lat_str);
                lv_textarea_set_text(ui_Longitude, lon_str);
                lv_label_set_text(ui_Location_Name, city->valuestring);
                lv_dropdown_set_selected(ui_Timezone_Dropdown, get_timezone_index(time_zone_id->valuestring));
            } else {
                ESP_LOGE(TAG, "Failed to extract location data from JSON");
            }
            cJSON_Delete(json);
        } else {
            ESP_LOGE(TAG, "Failed to parse JSON response");
        }
    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

void keypad_ready(lv_event_t *e)
{
    // Clear city name when keypad is used
    lv_label_set_text(ui_Location_Name, "");
}