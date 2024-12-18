#include "weather.h"
#include "azan_clock.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "ui/ui.h"
#include "lvgl/lvgl.h"
#include "esp_system.h"

#define TAG "Weather"
#define WEATHER_API_URL "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current_weather=true"
#define MIN(a,b) ((a) < (b) ? (a) : (b))

static char weather_info[512];
static size_t weather_info_len = 0; // Add this variable to keep track of buffer length
extern lv_obj_t *ui_Weather_Image;

// Define a structure for mapping weather codes to images
typedef struct {
    int code;
    const lv_img_dsc_t* image;
} WeatherCodeImageMap;

// Create the WMO weather code mapping array
static const WeatherCodeImageMap weather_map_day[] = {
    { 0, &ui_img_day_clear_png },
    { 1, &ui_img_day_clear_png },
    { 2, &ui_img_day_partly_cloudy_png },
    { 3, &ui_img_day_overcast_png },
    { 45, &ui_img_day_fog_png },
    { 48, &ui_img_day_rime_fog_png },
    { 51, &ui_img_day_light_drizzle_png },
    { 53, &ui_img_day_light_drizzle_png },
    { 55, &ui_img_day_light_drizzle_png },
    { 55, &ui_img_day_light_drizzle_png },
    { 55, &ui_img_day_light_drizzle_png },
    { 61, &ui_img_day_light_rain_png },
    { 63, &ui_img_day_moderate_rain_png },
    { 65, &ui_img_day_heavy_rain_png },
    { 61, &ui_img_day_light_rain_png },
    { 65, &ui_img_day_heavy_rain_png },
    { 71, &ui_img_day_heavy_snowfall_png },
    { 73, &ui_img_day_heavy_snowfall_png },
    { 75, &ui_img_day_heavy_snowfall_png },
    { 77, &ui_img_day_snowflake_png },
    { 80, &ui_img_day_light_rain_png },
    { 81, &ui_img_day_moderate_rain_png },
    { 82, &ui_img_day_heavy_rain_png },
    { 85, &ui_img_day_heavy_snowfall_png },
    { 86, &ui_img_day_heavy_snowfall_png },
    { 95, &ui_img_day_thunderstorm_png },
    { 96, &ui_img_day_thunderstorm_png },
    { 99, &ui_img_day_thunderstorm_png },
};

static const WeatherCodeImageMap weather_map_night[] = {
    { 0, &ui_img_night_clear_png },
    { 1, &ui_img_night_clear_png },
    { 2, &ui_img_night_partly_cloudy_png },
    { 3, &ui_img_night_overcast_png },
    { 45, &ui_img_night_fog_png },
    { 48, &ui_img_night_fog_png },
    { 51, &ui_img_night_light_drizzle_png },
    { 53, &ui_img_night_light_drizzle_png },
    { 55, &ui_img_night_light_drizzle_png },
    { 55, &ui_img_night_light_drizzle_png },
    { 55, &ui_img_night_light_drizzle_png },
    { 61, &ui_img_night_light_rain_png },
    { 63, &ui_img_night_light_rain_png },
    { 65, &ui_img_night_heavy_rain_png },
    { 63, &ui_img_night_light_rain_png },
    { 65, &ui_img_night_heavy_rain_png },
    { 71, &ui_img_night_heavy_snowfall_png },
    { 73, &ui_img_night_heavy_snowfall_png },
    { 75, &ui_img_night_heavy_snowfall_png },
    { 77, &ui_img_night_snowflake_png },
    { 80, &ui_img_night_light_rain_png },
    { 81, &ui_img_night_light_rain_png },
    { 82, &ui_img_night_heavy_rain_png },
    { 85, &ui_img_night_heavy_snowfall_png },
    { 86, &ui_img_night_heavy_snowfall_png },
    { 95, &ui_img_night_thunderstorm_png },
    { 96, &ui_img_night_thunderstorm_png },
    { 99, &ui_img_night_thunderstorm_png },
};

// Function to get the image resource for a given weather code
const lv_img_dsc_t* get_weather_image(int is_day, int weather_code) {
    const WeatherCodeImageMap* weather_map = is_day ? weather_map_day : weather_map_night;
    size_t map_size = is_day ? sizeof(weather_map_day) / sizeof(weather_map_day[0]) :
        sizeof(weather_map_night) / sizeof(weather_map_night[0]);
    for (size_t i = 0; i < map_size; ++i) {
        if (weather_map[i].code == weather_code) {
            return weather_map[i].image;
        }
    }
    // Return a default image if code not found
    return &ui_img_day_partly_cloudy_png; // Use a default image as fallback
}

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

esp_err_t get_weather_forecast(float latitude, float longitude) {
    // Change return type to esp_err_t
    esp_err_t result = ESP_FAIL;
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
                cJSON *is_day = cJSON_GetObjectItem(current_weather, "is_day");

                if (temperature != NULL && windspeed != NULL && weathercode != NULL) {
                    ESP_LOGI(TAG, "Current temperature: %.2f°C", temperature->valuedouble);
                    ESP_LOGI(TAG, "Current windspeed: %.2f km/h", windspeed->valuedouble);
                    ESP_LOGI(TAG, "Current weathercode: %d", weathercode->valueint);
                    ESP_LOGI(TAG, "Current is_day: %d", is_day->valueint);

                    // Get the image corresponding to the weather code
                    const lv_img_dsc_t* weather_image = get_weather_image(is_day->valueint, weathercode->valueint);

                    // Update the weather image on the UI
                    take_ui_mutex("get_weather_forecast");
                    lv_img_set_src(ui_Weather_Image, weather_image);
                    give_ui_mutex("get_weather_forecast");
                }
            }
            cJSON_Delete(json);
        }
        result = ESP_OK;
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        result = err;
    }

    esp_http_client_cleanup(client);
    return result;
}

static void weather_update_task(void *arg) {
    const TickType_t THREE_HOURS = pdMS_TO_TICKS(3 * 60 * 60 * 1000); // 3 hours in milliseconds
    const TickType_t MIN_RETRY_DELAY = pdMS_TO_TICKS(60 * 1000);      // 1 minute
    const TickType_t MAX_RETRY_DELAY = pdMS_TO_TICKS(30 * 60 * 1000); // 30 minutes
    TickType_t retry_delay = MIN_RETRY_DELAY;

    while (true) {
        // Retrieve weather forecast
        esp_err_t result = get_weather_forecast(40.7128, -74.0060); // Example coordinates for New York City
        
        if (result == ESP_OK) {
            // Success - reset retry delay and wait for full interval
            retry_delay = MIN_RETRY_DELAY;
            vTaskDelay(THREE_HOURS);
        } else {
            // On failure, use exponential backoff
            ESP_LOGW(TAG, "Weather update failed, retrying in %lu ms", pdTICKS_TO_MS(retry_delay));
            vTaskDelay(retry_delay);
            retry_delay = MIN(retry_delay * 2, MAX_RETRY_DELAY);
        }
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
