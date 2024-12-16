#include "clock.h"
#include "azan_clock.h" // Include for the externally declared LVGL label ui_Current_Time
#include <time.h>
#include <sys/time.h>
#include <esp_log.h>
#include <esp_sntp.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "Clock"
#define DEFAULT_TIMEZONE "EST5EDT" // Default timezone for America/New_York

#define NTP_SERVER_COUNT 4
const char *ntp_servers[NTP_SERVER_COUNT] = {
    "192.168.50.243",
    "time.google.com",
    "pool.ntp.org",
    "time.nist.gov",
};

static char current_timezone[64] = DEFAULT_TIMEZONE;
TaskHandle_t time_update_task_handle = NULL; // Add this line

// Function to synchronize time with NTP server
static void sync_time_with_ntp(lv_obj_t *sync_msgbox) {
    ESP_LOGI(TAG, "Initializing SNTP...");

    esp_sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);

    int server_index = 0;
    int retry = 0;
    const int retry_count = 5;
    bool sync_successful = false;

    while (server_index < NTP_SERVER_COUNT && !sync_successful) {
        ESP_LOGI(TAG, "Trying NTP server: %s", ntp_servers[server_index]);
        esp_sntp_setservername(0, ntp_servers[server_index]);
        esp_sntp_init();

        retry = 0;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
            ESP_LOGI(TAG, "Waiting for system time to be set (%s)... (%d/%d)", ntp_servers[server_index], retry, retry_count);
            vTaskDelay(1000 / portTICK_PERIOD_MS); // Reduced delay to 1 second
        }

        if (sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED) {
            ESP_LOGI(TAG, "System time synchronized successfully with server: %s", ntp_servers[server_index]);
            sync_successful = true;
            lv_obj_del(sync_msgbox); // Remove the synchronization messagebox
        } else {
            ESP_LOGE(TAG, "Failed to synchronize time with NTP server: %s", ntp_servers[server_index]);
            esp_sntp_stop(); // Stop SNTP before trying the next server
            server_index++;
        }
    }

    if (!sync_successful) {
        ESP_LOGE(TAG, "Failed to synchronize time with all NTP servers.");
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
    lv_obj_t *sync_msgbox = (lv_obj_t *)arg;
    while (true) {
        sync_time_with_ntp(sync_msgbox);
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

    // Create the synchronization messagebox
    lv_obj_t *sync_msgbox = lv_msgbox_create(NULL, "Synchronizing ...", NULL, NULL, true);
    lv_obj_set_size(sync_msgbox, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_opa(sync_msgbox, LV_OPA_50, 0);
    lv_obj_set_style_bg_color(sync_msgbox, lv_color_black(), 0);
    lv_obj_center(sync_msgbox);

    // Start the tasks
    xTaskCreate(ntp_sync_task, "ntp_sync_task", 4096, sync_msgbox, 5, NULL);
    xTaskCreate(time_update_task, "time_update_task", 4096, NULL, 5, &time_update_task_handle); // Pass the handle
}