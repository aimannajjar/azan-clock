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

// System Time
#include "systime.h"

#define TAG "Main"

LV_FONT_DECLARE(noto_naskh_80)

QueueHandle_t ui_update_queue;
SemaphoreHandle_t lvgl_mutex;

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

    bool setup_mode = !wifi_init();
    ui_init();

    lvgl_mutex = xSemaphoreCreateMutex();

    if (setup_mode) {
        lv_scr_load(ui_Setup_Screen);
    }
    clock_init();
    systime_init();
}

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
