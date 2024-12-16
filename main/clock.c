#include "clock.h"
#include "azan_clock.h" // Include for the externally declared LVGL label ui_Current_Time
#include <time.h>
#include <sys/time.h>
#include <esp_log.h>
#include <esp_sntp.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "Clock"
#define NTP_SERVER "pool.ntp.org"

// Function to synchronize time with NTP server
static void sync_time_with_ntp(void) {
    ESP_LOGI(TAG, "Initializing SNTP...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER);
    sntp_init();

    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (1970 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year >= (1970 - 1900)) {
        ESP_LOGI(TAG, "System time synchronized successfully.");
    } else {
        ESP_LOGE(TAG, "Failed to synchronize time with NTP.");
    }
}

// Task to periodically sync with NTP server every 4 hours
static void ntp_sync_task(void *arg) {
    while (true) {
        sync_time_with_ntp();
        vTaskDelay(pdMS_TO_TICKS(14400000)); // 4 hours
    }
}

// Task to update the LVGL label every minute
static void time_update_task(void *arg) {
    while (true) {
        time_t now;
        struct tm timeinfo;
        char time_str[64];

        time(&now);
        localtime_r(&now, &timeinfo);

        if (timeinfo.tm_hour >= 12) {
            snprintf(time_str, sizeof(time_str), "%02d:%02d PM",
                     timeinfo.tm_hour > 12 ? timeinfo.tm_hour - 12 : timeinfo.tm_hour,
                     timeinfo.tm_min);
        } else {
            snprintf(time_str, sizeof(time_str), "%02d:%02d AM",
                     timeinfo.tm_hour == 0 ? 12 : timeinfo.tm_hour,
                     timeinfo.tm_min);
        }

        lv_label_set_text(ui_Current_Time, time_str); // Update LVGL label
        ESP_LOGI(TAG, "Time updated: %s", time_str);

        vTaskDelay(pdMS_TO_TICKS(60000)); // 1 minute
    }
}

void clock_init(void) {
    xTaskCreate(ntp_sync_task, "ntp_sync_task", 4096, NULL, 5, NULL);
    xTaskCreate(time_update_task, "time_update_task", 4096, NULL, 5, NULL);
}
