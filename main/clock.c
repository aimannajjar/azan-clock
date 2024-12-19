#include "clock.h"
#include "azan_clock.h"
#include <time.h>
#include <sys/time.h>
#include <esp_log.h>
#include <esp_sntp.h>
#include <esp_netif_sntp.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "Clock"

extern lv_obj_t *ui_Next_Prayer_Panel;

// External LVGL container references
extern lv_obj_t *ui_Fajr_Container;
extern lv_obj_t *ui_Sunrise_Container;
extern lv_obj_t *ui_Duhur_Container;
extern lv_obj_t *ui_Asr_Container;
extern lv_obj_t *ui_Maghrib_Container;
extern lv_obj_t *ui_Isha_Container;

// External LVGL label references
extern lv_obj_t *ui_Current_Time;
extern lv_obj_t *ui_Current_Time1;
extern lv_obj_t *ui_Fajr_Time;
extern lv_obj_t *ui_Sunrise_Time;
extern lv_obj_t *ui_Duhur_Time;
extern lv_obj_t *ui_Asr_Time;
extern lv_obj_t *ui_Maghrib_Time;
extern lv_obj_t *ui_Isha_Time;

// Helper function to convert prayer time string to minutes since midnight
static int get_prayer_minutes(const char* prayer_time) {
    int hour, minute;
    char period[3];
    if (sscanf(prayer_time, "%d:%d %2s", &hour, &minute, period) == 3) {
        if (strcmp(period, "PM") == 0 && hour != 12) hour += 12;
        if (strcmp(period, "AM") == 0 && hour == 12) hour = 0;
        return hour * 60 + minute;
    }
    return -1;
}

// Updates time in UI
static void update_time_ui() {
    time_t now;
    struct tm timeinfo;
    char time_str[64];

    time(&now);
    localtime_r(&now, &timeinfo);

    // Calculate current time in minutes since midnight
    int current_minutes = timeinfo.tm_hour * 60 + timeinfo.tm_min;

    // Format current time display
    if (timeinfo.tm_hour >= 12) {
        snprintf(time_str, sizeof(time_str), "%02d:%02d\nPM",
                 timeinfo.tm_hour > 12 ? timeinfo.tm_hour - 12 : timeinfo.tm_hour,
                 timeinfo.tm_min);
    } else {
        snprintf(time_str, sizeof(time_str), "%02d:%02d\nAM",
                 timeinfo.tm_hour == 0 ? 12 : timeinfo.tm_hour,
                 timeinfo.tm_min);
    }

    lv_label_set_text(ui_Current_Time, time_str);
    lv_label_set_text(ui_Current_Time1, time_str);

    // Setup prayer times array
    struct {
        lv_obj_t *container;
        lv_obj_t *time_label;
    } prayers[] = {
        {ui_Fajr_Container, ui_Fajr_Time},
        {ui_Sunrise_Container, ui_Sunrise_Time},
        {ui_Duhur_Container, ui_Duhur_Time},
        {ui_Asr_Container, ui_Asr_Time},
        {ui_Maghrib_Container, ui_Maghrib_Time},
        {ui_Isha_Container, ui_Isha_Time}
    };

    // Reset all containers to default color
    for (int i = 0; i < 6; i++) {
        lv_obj_set_style_bg_color(prayers[i].container, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(prayers[i].container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // Find next prayer time
    int next_prayer_idx = -1;
    int min_time_diff = 24 * 60; // Initialize to maximum minutes in a day

    for (int i = 0; i < 6; i++) {
        const char* prayer_time = lv_label_get_text(prayers[i].time_label);
        int prayer_minutes = get_prayer_minutes(prayer_time);
        
        if (prayer_minutes >= 0) {
            int time_diff = prayer_minutes - current_minutes;
            if (time_diff < 0) time_diff += 24 * 60; // Add 24 hours if prayer time has passed
            
            if (time_diff < min_time_diff) {
                min_time_diff = time_diff;
                next_prayer_idx = i;
            }
        }
    }

    // Highlight next prayer time container
    if (next_prayer_idx >= 0) {
        lv_obj_set_style_bg_color(prayers[next_prayer_idx].container, lv_color_hex(0xFFD29C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(prayers[next_prayer_idx].container, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        ESP_LOGI(TAG, "Next prayer highlighted: %d, time diff: %d minutes", next_prayer_idx, min_time_diff);
    }

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
