#include "weather.h"
#include "azan_clock.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "ui/ui.h"

#define TAG "Weather"
#define WEATHER_API_URL "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current_weather=true"

static char weather_info[1024];
static size_t weather_info_len = 0; // Add this variable to keep track of buffer length
extern lv_obj_t *ui_Weather_Image;

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
            // Append received data to weather_info buffer
            if (evt->data_len + weather_info_len < sizeof(weather_info)) {
                memcpy(weather_info + weather_info_len, evt->data, evt->data_len);
                weather_info_len += evt->data_len;
                weather_info[weather_info_len] = '\0'; // Null-terminate
            } else {
                ESP_LOGW(TAG, "weather_info buffer is full, cannot append more data");
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

void get_weather_forecast(float latitude, float longitude) {
    char url[256];
    snprintf(url, sizeof(url), WEATHER_API_URL, latitude, longitude);

    // Clear weather_info buffer and reset length before starting request
    memset(weather_info, 0, sizeof(weather_info));
    weather_info_len = 0;

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        .cert_pem = NULL, // Use the default certificate bundle
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %lld",
                 esp_http_client_get_status_code(client),
                 (long long)esp_http_client_get_content_length(client));

        ESP_LOGI(TAG, "Weather JSON response: %s", weather_info); // Log the JSON response

        cJSON *json = cJSON_Parse(weather_info);
        if (json == NULL) {
            ESP_LOGE(TAG, "Failed to parse JSON response");
        } else {
            cJSON *current_weather = cJSON_GetObjectItem(json, "current_weather");
            if (current_weather != NULL) {
                cJSON *temperature = cJSON_GetObjectItem(current_weather, "temperature");
                cJSON *windspeed = cJSON_GetObjectItem(current_weather, "windspeed");
                cJSON *weathercode = cJSON_GetObjectItem(current_weather, "weathercode");
                if (temperature != NULL && windspeed != NULL) {
                    ESP_LOGI(TAG, "Current temperature: %.2f°C", temperature->valuedouble);
                    ESP_LOGI(TAG, "Current windspeed: %.2f km/h", windspeed->valuedouble);
                    ESP_LOGI(TAG, "Current weathercode: %d km/h", weathercode->valueint);

                }
            }
            cJSON_Delete(json);

            take_ui_mutex("get_weather_forecast");
            lv_img_set_src(ui_Weather_Image, &ui_img_day_clear_png);
            give_ui_mutex("get_weather_forecast");
        }
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

static void weather_update_task(void *arg) {
    while (true) {
        // Retrieve weather forecast
        get_weather_forecast(40.7128, -74.0060); // Example coordinates for New York City
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000)); // Wait for notification or 1 minute
    }
}

// Function to initialize the weather module
void weather_init(void) {
    if (is_weather_initialized()) {
        ESP_LOGI(TAG, "Weather already initialized, skipping...");
        return;
    }
    
    xTaskCreate(weather_update_task, "weather_update_task", 4096, NULL, 5, NULL);
    set_weather_initialized();
    ESP_LOGI(TAG, "Weather initialized successfully");
}
