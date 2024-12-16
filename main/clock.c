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
#define DEFAULT_TIMEZONE "EST5EDT" // Default timezone for America/New_York

static char current_timezone[64] = DEFAULT_TIMEZONE;
TaskHandle_t time_update_task_handle = NULL; // Add this line

// Function to synchronize time with NTP server
static void sync_time_with_ntp(void) {
    ESP_LOGI(TAG, "Initializing SNTP...");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, NTP_SERVER);
    sntp_init();

    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 20;

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

// Updates time in UI
static void update_time() {
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

// Task to periodically sync with NTP server every 4 hours
static void ntp_sync_task(void *arg) {
    while (true) {
        sync_time_with_ntp();
        xTaskNotifyGive(time_update_task_handle); // Notify the time update task
        vTaskDelay(pdMS_TO_TICKS(14400000)); // 4 hours
    }
}

// Task to update the LVGL label every minute
static void time_update_task(void *arg) {
    while (true) {
        update_time();
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(60000)); // Wait for notification or 1 minute
    }
}

// Function to set the timezone
void clock_set_timezone(const char *timezone) {
    if (timezone && strlen(timezone) < sizeof(current_timezone)) {
        strcpy(current_timezone, timezone);
        ESP_LOGI(TAG, "Timezone set to: %s", current_timezone);
    } else {
        ESP_LOGW(TAG, "Invalid timezone specified. Using default timezone: %s", DEFAULT_TIMEZONE);
        strcpy(current_timezone, DEFAULT_TIMEZONE);
    }

    // Apply the timezone
    setenv("TZ", current_timezone, 1);
    tzset();
}

// Function to initialize the clock module
void clock_init(void) {
    // Set the default timezone
    clock_set_timezone(DEFAULT_TIMEZONE);

    // Start the tasks
    xTaskCreate(ntp_sync_task, "ntp_sync_task", 4096, NULL, 5, NULL);
    xTaskCreate(time_update_task, "time_update_task", 4096, NULL, 5, &time_update_task_handle); // Pass the handle
}