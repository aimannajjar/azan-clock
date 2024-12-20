#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_log.h"
#include "cJSON.h"
#include "azan_clock.h"
#include "lvgl/lvgl.h"
#include "timezones.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "prayers.h"
#include "settings.h"

#define TAG "Settings"
#define SETTINGS_NAMESPACE "settings"
#define MAX_STR_LEN 64
#define FLOAT_BLOB_SIZE sizeof(float)

extern lv_obj_t *ui_Latitude;
extern lv_obj_t *ui_Longitude;
extern lv_obj_t *ui_Location_Name;
extern lv_obj_t *ui_Timezone_Dropdown;
extern lv_obj_t *ui_Calculation_Method_Dropdown;
extern lv_obj_t *ui_Keypad;
extern lv_obj_t *ui_Settings_Screen;

void first_time_settings() {
    take_ui_mutex("first_time_settings");
    lv_scr_load(ui_Settings_Screen);
    give_ui_mutex("first_time_settings");
}

void settings_init(void) {
    ESP_LOGI(TAG, "Initializing settings");
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(SETTINGS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return;
    }

    // Check latitude using blob
    float latitude;
    size_t blob_size = FLOAT_BLOB_SIZE;
    err = nvs_get_blob(handle, "latitude", &latitude, &blob_size);
    if (err != ESP_OK || blob_size != FLOAT_BLOB_SIZE) {
        ESP_LOGW(TAG, "Latitude not found in settings");
        first_time_settings();
        return;
    }

    // Check longitude using blob
    float longitude;
    blob_size = FLOAT_BLOB_SIZE;
    err = nvs_get_blob(handle, "longitude", &longitude, &blob_size);
    if (err != ESP_OK || blob_size != FLOAT_BLOB_SIZE) {
        ESP_LOGW(TAG, "Longitude not found in settings");
        first_time_settings();
        return;
    }

    // Check timezone
    uint16_t tz_index;
    err = nvs_get_u16(handle, "timezone", &tz_index);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Timezone not found in settings");
        first_time_settings();
        return;
    }

    // Check calculation method
    uint8_t calc_method;
    err = nvs_get_u8(handle, "calc_method", &calc_method);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Calculation method not found in settings");
        first_time_settings();
        return;
    }

    nvs_close(handle);

    // Load all settings into UI components
    err = load_settings();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to load settings into UI: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Settings loaded successfully");
    }
}

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

// Load settings from NVS
esp_err_t load_settings(void) {
    ESP_LOGI(TAG, "Loading settings from NVS");
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(SETTINGS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    take_ui_mutex("load_settings");
    size_t blob_size = FLOAT_BLOB_SIZE;

    // Load latitude
    float latitude = 0.0f;
    err = nvs_get_blob(handle, "latitude", &latitude, &blob_size);
    if (err == ESP_OK) {
        char lat_str[16];
        snprintf(lat_str, sizeof(lat_str), "%f", latitude);
        lv_textarea_set_text(ui_Latitude, lat_str);
        ESP_LOGI(TAG, "Loaded latitude: %f", latitude);
    }

    // Load longitude
    float longitude = 0.0f;
    err = nvs_get_blob(handle, "longitude", &latitude, &blob_size);
    if (err == ESP_OK) {
        char lon_str[16];
        snprintf(lon_str, sizeof(lon_str), "%f", longitude);
        lv_textarea_set_text(ui_Longitude, lon_str);
        ESP_LOGI(TAG, "Loaded longitude: %f", longitude);
    }

    // Load city name
    char city_name[MAX_STR_LEN] = {0};
    size_t city_len = sizeof(city_name);
    err = nvs_get_str(handle, "city_name", city_name, &city_len);
    if (err == ESP_OK) {
        lv_label_set_text(ui_Location_Name, city_name);
        ESP_LOGI(TAG, "Loaded city name: %s", city_name);
    }

    // Load timezone index
    uint16_t tz_index = 0;
    err = nvs_get_u16(handle, "timezone", &tz_index);
    if (err == ESP_OK) {
        lv_dropdown_set_selected(ui_Timezone_Dropdown, tz_index);
        ESP_LOGI(TAG, "Loaded timezone index: %d", tz_index);
    }

    // Load calculation method
    uint8_t calc_method = 0;
    err = nvs_get_u8(handle, "calc_method", &calc_method);
    if (err == ESP_OK) {
        lv_dropdown_set_selected(ui_Calculation_Method_Dropdown, calc_method);
        ESP_LOGI(TAG, "Loaded calculation method: %d", calc_method);
    }
    give_ui_mutex("load_settings");

    if (err == ESP_OK) {
        // Update state after successful load
        set_current_latitude(latitude);
        set_current_longitude(longitude);
        set_current_city(city_name);
        set_current_timezone(tz_index);
        set_current_calculation_method(calc_method);
        notify_prayers();
        ESP_LOGI(TAG, "Updated application state with loaded settings");
    }

    nvs_close(handle);
    return ESP_OK;
}

// Save settings to NVS
esp_err_t save_settings(void) {
    ESP_LOGI(TAG, "Saving settings to NVS");
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(SETTINGS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    // Save latitude
    float latitude = atof(lv_textarea_get_text(ui_Latitude));
    err = nvs_set_blob(handle, "latitude", &latitude, FLOAT_BLOB_SIZE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error saving latitude: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    ESP_LOGI(TAG, "Saved latitude: %f", latitude);

    // Save longitude using blob
    float longitude = atof(lv_textarea_get_text(ui_Longitude));
    err = nvs_set_blob(handle, "longitude", &longitude, FLOAT_BLOB_SIZE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error saving longitude: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }
    ESP_LOGI(TAG, "Saved longitude: %f", longitude);

    // Save city name
    const char* city_name = lv_label_get_text(ui_Location_Name);
    err = nvs_set_str(handle, "city_name", city_name);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error saving city name: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }
    ESP_LOGI(TAG, "Saved city name: %s", city_name);

    // Save timezone index
    uint16_t tz_index = lv_dropdown_get_selected(ui_Timezone_Dropdown);
    err = nvs_set_u16(handle, "timezone", tz_index);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error saving timezone: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }
    ESP_LOGI(TAG, "Saved timezone index: %d", tz_index);

    // Save calculation method
    uint8_t calc_method = lv_dropdown_get_selected(ui_Calculation_Method_Dropdown);
    err = nvs_set_u8(handle, "calc_method", calc_method);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error saving calculation method: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }
    ESP_LOGI(TAG, "Saved calculation method: %d", calc_method);

    if (err == ESP_OK) {
        // Update state after successful save
        float latitude = atof(lv_textarea_get_text(ui_Latitude));
        float longitude = atof(lv_textarea_get_text(ui_Longitude));
        const char* city_name = lv_label_get_text(ui_Location_Name);
        uint16_t tz_index = lv_dropdown_get_selected(ui_Timezone_Dropdown);
        uint8_t calc_method = lv_dropdown_get_selected(ui_Calculation_Method_Dropdown);

        set_current_latitude(latitude);
        set_current_longitude(longitude);
        set_current_city(city_name);
        set_current_timezone(tz_index);
        set_current_calculation_method(calc_method);
        if (is_prayers_initialized()) {
            notify_prayers();
        } else {
            prayers_init();
        }
        if (is_weather_initialized()) {
            notify_weather();
        } else {
            weather_init();
        }
        ESP_LOGI(TAG, "Updated application state with saved settings");
    }

    // Commit changes
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error committing settings: %s", esp_err_to_name(err));
        nvs_close(handle);
        return err;
    }

    nvs_close(handle);
    ESP_LOGI(TAG, "Settings saved successfully");
    return ESP_OK;
}
