#include "lvgl/lvgl.h"
#include "azan_clock.h"
#include "ui/ui.h"

// Wi-FI
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "clock.h"

#define TAG "Main"

LV_FONT_DECLARE(noto_naskh_80)

SemaphoreHandle_t lvgl_mutex;

typedef struct {
    bool initialized;
} state_t;

state_t state = { .initialized = false };

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
    lvgl_mutex = xSemaphoreCreateMutex();

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

void set_system_initialized() {
    state.initialized = true;
}

bool is_system_initialized() {
    return state.initialized;
}

void take_ui_mutex(const char *caller) {
    ESP_LOGI(TAG, "UI Mutex: Taking from %s", caller);
    xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
}

void give_ui_mutex(const char *caller) {
    ESP_LOGI(TAG, "UI Mutex: Giving from %s", caller);
    xSemaphoreGive(lvgl_mutex);
}
