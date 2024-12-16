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
#define DEFAULT_TIMEZONE "EST5EDT" // Default timezone for America/New_York

#define NTP_SERVER_COUNT 3
#define NTP_SERVERS "time.google.com", "time.nist.gov", "pool.ntp.org"

static char current_timezone[64] = DEFAULT_TIMEZONE;

// // Callback function to log the progress of NTP sync status
// void time_sync_notification_cb(struct timeval *tv) {
//     ESP_LOGI(TAG, "Time synchronization event received");
// }

// Function to synchronize time with NTP server
// static void sync_time_with_ntp(lv_obj_t *sync_msgbox) {
//     ESP_LOGI(TAG, "Initializing SNTP...");

//     esp_sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
//     sntp_set_time_sync_notification_cb(time_sync_notification_cb);

//     esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(NTP_SERVER_COUNT,
//                 ESP_SNTP_SERVER_LIST(NTP_SERVERS));

//     esp_netif_sntp_init(&config);

//     // wait for time to be set
//     time_t now = 0;
//     struct tm timeinfo = { 0 };
//     int retry = 0;
//     const int retry_count = 15;
//     // bool sync_successful = false;
//     while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count) {
//         ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
//     }
//     time(&now);
//     localtime_r(&now, &timeinfo);

//     // ESP_ERROR_CHECK( example_disconnect() );
//     esp_netif_sntp_deinit();


//     // while (server_index < NTP_SERVER_COUNT && !sync_successful) {
//     //     ESP_LOGI(TAG, "Trying NTP server: %s", ntp_servers[server_index]);
//     //     esp_sntp_setservername(0, ntp_servers[server_index]);
//     //     esp_sntp_init();

//     //     retry = 0;
//     //     while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
//     //         ESP_LOGI(TAG, "Waiting for system time to be set (%s)... (%d/%d)", ntp_servers[server_index], retry, retry_count);
//     //         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     //     }

//     //     ESP_LOGI(TAG, "SNTP STATUS: %d", sntp_get_sync_status());

//     //     if (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS) {
//     //         retry = 0;
//     //         while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS && ++retry < retry_count) {
//     //             ESP_LOGI(TAG, "Sync completed. Waiting for system time to adjust");
//     //             vTaskDelay(1000 / portTICK_PERIOD_MS);
//     //         }    
//     //     }

//     //     ESP_LOGI(TAG, "SNTP STATUS: %d", sntp_get_sync_status());

//     //     if (sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED) {
//     //         ESP_LOGI(TAG, "System time synchronized successfully with server: %s", ntp_servers[server_index]);
//     //         sync_successful = true;
//     //         lv_obj_del(sync_msgbox); // Remove the synchronization messagebox
//     //     } else {
//     //         ESP_LOGE(TAG, "Failed to synchronize time with NTP server: %s", ntp_servers[server_index]);
//     //         esp_sntp_stop(); // Stop SNTP before trying the next server
//     //         server_index++;
//     //     }
//     // }

//     // if (!sync_successful) {
//     //     ESP_LOGE(TAG, "Failed to synchronize time with all NTP servers.");
//     // }
// }

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

// // Task to periodically sync with NTP server every 4 hours
// static void ntp_sync_task(void *arg) {
//     lv_obj_t *sync_msgbox = (lv_obj_t *)arg;
//     while (true) {
//         sync_time_with_ntp(sync_msgbox);
//         xTaskNotifyGive(time_update_task_handle); // Notify the time update task
//         vTaskDelay(pdMS_TO_TICKS(14400000)); // 4 hours
//     }
// }

// Task to update the LVGL label every minute
static void time_update_task(void *arg) {
    while (true) {
        update_time_ui();
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000)); // Wait for notification or 1 minute
    }
}

// Function to initialize the clock module
void clock_init(void) {

    // // Create the synchronization messagebox
    // lv_obj_t *sync_msgbox = lv_msgbox_create(NULL, "Synchronizing ...", NULL, NULL, true);
    // lv_obj_set_size(sync_msgbox, LV_HOR_RES, LV_VER_RES);
    // lv_obj_set_style_bg_opa(sync_msgbox, LV_OPA_50, 0);
    // lv_obj_set_style_bg_color(sync_msgbox, lv_color_black(), 0);
    // lv_obj_center(sync_msgbox);

    // Start the tasks
    xTaskCreate(time_update_task, "time_update_task", 4096, NULL, 5, NULL); // Pass the handle
}