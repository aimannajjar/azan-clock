#include "lvgl/lvgl.h"
#include "azan_clock.h"
#include "ui/ui.h"
#include <sys/time.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "weather.h" // Include the weather header

#define TAG "Main"

LV_FONT_DECLARE(noto_naskh_80)

extern SemaphoreHandle_t lvgl_mux;

typedef struct {
    bool wifi_previously_connected;
    bool clock_initialized;
    bool weather_initialized;
    bool prayers_initialized;
    float latitude;
    float longitude;
    char city[64];
    uint16_t timezone;
    uint8_t calculation_method;
} state_t;

state_t state = { .wifi_previously_connected = false, .clock_initialized = false };

extern lv_obj_t *ui_Loading_Status_Text;
void azan_clock() {
    // Initialize NVS (needed for Wi-Fi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ui_init();
    wifi_init();
}

void reset_nvs() {
    esp_err_t ret = nvs_flash_erase();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to erase NVS: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "NVS erased successfully");
    }
}

void set_wifi_previously_connected() {
    state.wifi_previously_connected = true;
}

bool is_wifi_previously_connected() {
    return state.wifi_previously_connected;
}

void set_clock_initialized() {
    state.clock_initialized = true;
}

bool is_clock_initialized() {
    return state.clock_initialized;
}

void set_weather_initialized() {
    state.weather_initialized = true;
}

bool is_weather_initialized() {
    return state.weather_initialized;
}

bool is_prayers_initialized(void) {
    return state.prayers_initialized;
}

void set_prayers_initialized(void) {
    state.prayers_initialized = true;
}

void set_current_latitude(float lat) {
    state.latitude = lat;
}

void set_current_longitude(float lon) {
    state.longitude = lon;
}

void set_current_city(const char* city) {
    strncpy(state.city, city, sizeof(state.city) - 1);
    state.city[sizeof(state.city) - 1] = '\0';
}

void set_current_timezone(uint16_t tz) {
    state.timezone = tz;
}

void set_current_calculation_method(uint8_t method) {
    state.calculation_method = method;
}

float get_current_latitude(void) {
    return state.latitude;
}

float get_current_longitude(void) {
    return state.longitude;
}

const char* get_current_city(void) {
    return state.city;
}

uint16_t get_current_timezone(void) {
    return state.timezone;
}

uint8_t get_current_calculation_method(void) {
    return state.calculation_method;
}

void take_ui_mutex(const char *caller) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ESP_LOGI(TAG, "[%lld.%06ld] UI Mutex: Taking from %s", (long long)tv.tv_sec, (long)tv.tv_usec, caller);
    xSemaphoreTake(lvgl_mux, portMAX_DELAY);
}

void give_ui_mutex(const char *caller) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ESP_LOGI(TAG, "[%lld.%06ld] UI Mutex: Giving from %s", (long long)tv.tv_sec, (long)tv.tv_usec, caller);
    xSemaphoreGive(lvgl_mux);
}
