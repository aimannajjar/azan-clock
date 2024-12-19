#include "prayers.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "ui/ui.h"
#include "lvgl/lvgl.h"
#include "azan_clock.h"

#define TAG "Prayers"
#define PRAYERS_API_URL "https://prayers.hurranet.net/prod/prayers?latitude=%f&longitude=%f&method=3"
#define MIN(a,b) ((a) < (b) ? (a) : (b))

static char prayers_info[1024];
static size_t prayers_info_len = 0;

// External LVGL label references
extern lv_obj_t *ui_Fajr_Time;
extern lv_obj_t *ui_Sunrise_Time;
extern lv_obj_t *ui_Duhur_Time;
extern lv_obj_t *ui_Asr_Time;
extern lv_obj_t *ui_Maghrib_Time;
extern lv_obj_t *ui_Isha_Time;

// Convert 24-hour format to 12-hour AM/PM format
static void convert_to_12hour(const char* time24, char* time12, size_t size) {
    int hour, minute;
    sscanf(time24, "%d:%d", &hour, &minute);
    
    const char* period = (hour >= 12) ? "PM" : "AM";
    if (hour > 12) hour -= 12;
    if (hour == 0) hour = 12;
    
    snprintf(time12, size, "%d:%02d %s", hour, minute, period);
}

static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "Received data chunk: %d bytes", evt->data_len);
            if (evt->data_len + prayers_info_len < sizeof(prayers_info)) {
                memcpy(prayers_info + prayers_info_len, evt->data, evt->data_len);
                prayers_info_len += evt->data_len;
                prayers_info[prayers_info_len] = '\0';
            } else {
                ESP_LOGE(TAG, "Buffer overflow prevented. Buffer size: %d, Attempted to write: %d", 
                    sizeof(prayers_info), evt->data_len + prayers_info_len);
            }
            break;
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER: %s: %s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        default:
            ESP_LOGI(TAG, "Unhandled HTTP Event: %d", evt->event_id);
            break;
    }
    return ESP_OK;
}

esp_err_t get_prayer_times(float latitude, float longitude) {
    esp_err_t result = ESP_FAIL;
    char url[256];
    char formatted_time[16];
    snprintf(url, sizeof(url), PRAYERS_API_URL, latitude, longitude);
    ESP_LOGI(TAG, "Requesting prayer times from: %s", url);

    memset(prayers_info, 0, sizeof(prayers_info));
    prayers_info_len = 0;

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
    };

    ESP_LOGI(TAG, "Initializing HTTP client");
    esp_http_client_handle_t client = esp_http_client_init(&config);
    
    ESP_LOGI(TAG, "Performing HTTP request");
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        int content_length = esp_http_client_get_content_length(client);
        ESP_LOGI(TAG, "HTTP request completed: Status = %d, Content-Length = %d", 
                 status_code, content_length);
        ESP_LOGI(TAG, "Response body: %s", prayers_info);

        ESP_LOGI(TAG, "Parsing JSON response");
        cJSON *json = cJSON_Parse(prayers_info);
        if (json == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                ESP_LOGE(TAG, "JSON Parse Error before: %s", error_ptr);
            }
        } else {
            const char* prayer_times[] = {
                "Fajr", "Sunrise", "Dhuhr", "Asr", "Maghrib", "Isha"
            };
            lv_obj_t* ui_labels[] = {
                ui_Fajr_Time, ui_Sunrise_Time, ui_Duhur_Time,
                ui_Asr_Time, ui_Maghrib_Time, ui_Isha_Time
            };

            for (int i = 0; i < 6; i++) {
                cJSON *time = cJSON_GetObjectItem(json, prayer_times[i]);
                if (time != NULL && time->valuestring != NULL) {
                    convert_to_12hour(time->valuestring, formatted_time, sizeof(formatted_time));
                    ESP_LOGI(TAG, "Prayer time %s: %s (converted from %s)", 
                            prayer_times[i], formatted_time, time->valuestring);
                    lv_label_set_text(ui_labels[i], formatted_time);
                } else {
                    ESP_LOGE(TAG, "Failed to get time for %s", prayer_times[i]);
                }
            }
            result = ESP_OK;
            cJSON_Delete(json);
        }
    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
        result = err;
    }

    ESP_LOGI(TAG, "Cleaning up HTTP client");
    esp_http_client_cleanup(client);
    return result;
}

static void prayers_update_task(void *arg) {
    const TickType_t TWELVE_HOURS = pdMS_TO_TICKS(12 * 60 * 60 * 1000);
    const TickType_t MIN_RETRY_DELAY = pdMS_TO_TICKS(60 * 1000);      // 1 minute
    const TickType_t MAX_RETRY_DELAY = pdMS_TO_TICKS(30 * 60 * 1000); // 30 minutes
    TickType_t retry_delay = MIN_RETRY_DELAY;

    while (true) {
        esp_err_t result = get_prayer_times(40.7128, -74.0060); // Example coordinates
        
        if (result == ESP_OK) {
            retry_delay = MIN_RETRY_DELAY;
            vTaskDelay(TWELVE_HOURS);
        } else {
            ESP_LOGW(TAG, "Prayer times update failed, retrying in %lu ms", pdTICKS_TO_MS(retry_delay));
            vTaskDelay(retry_delay);
            retry_delay = MIN(retry_delay * 2, MAX_RETRY_DELAY);
        }
    }
}

void prayers_init(void) {
    if (is_prayers_initialized()) {
        ESP_LOGI(TAG, "Prayers already initialized, skipping...");
        return;
    }
    
    xTaskCreate(prayers_update_task, "prayers_update_task", 4096, NULL, 5, NULL);
    set_prayers_initialized();
    ESP_LOGI(TAG, "Prayer times service initialized successfully");
}
