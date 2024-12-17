#include "clock.h"
#include "azan_clock.h" // Include for the externally declared LVGL label ui_Current_Time
#include <time.h>
#include <sys/time.h>
#include <esp_log.h>
#include <esp_sntp.h>
#include <esp_netif_sntp.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "Clock"

extern SemaphoreHandle_t lvgl_mutex;

// Updates time in UI
static void update_time_ui() {
    time_t now;
    struct tm timeinfo;
    char time_str[64];

    time(&now);
    localtime_r(&now, &timeinfo);

    if (timeinfo.tm_hour >= 12) {
        snprintf(time_str, sizeof(time_str), "%02d:%02d\nPM",
                 timeinfo.tm_hour > 12 ? timeinfo.tm_hour - 12 : timeinfo.tm_hour,
                 timeinfo.tm_min);
    } else {
        snprintf(time_str, sizeof(time_str), "%02d:%02d\nAM",
                 timeinfo.tm_hour == 0 ? 12 : timeinfo.tm_hour,
                 timeinfo.tm_min);
    }

    lv_label_set_text(ui_Current_Time, time_str); // Update LVGL label

    ESP_LOGI(TAG, "Time updated: %s", time_str);
}

// Task to update the LVGL label every minute
static void time_update_task(void *arg) {
    while (true) {
        take_ui_mutex("time_update_task");
        update_time_ui();
        give_ui_mutex("time_update_task");
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000)); // Wait for notification or 1 minute
    }
}

// Function to initialize the clock module
void clock_init(void) {
    if (is_clock_initialized()) {
        ESP_LOGI(TAG, "Clock already initialized, skipping...");
        return;
    }
    
    xTaskCreate(time_update_task, "time_update_task", 4096, NULL, 5, NULL);
    set_clock_initialized();
    ESP_LOGI(TAG, "Clock initialized successfully");
}
