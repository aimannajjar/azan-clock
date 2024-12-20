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

// Add task handle
static TaskHandle_t time_update_task_handle = NULL;

extern lv_obj_t *ui_Next_Prayer_Panel;
extern lv_obj_t *ui_Next_Prayer_Panel1;
extern lv_obj_t *ui_Loading_Status_Text;
extern lv_obj_t *ui_Main_Screen;

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

// Add these external references
extern lv_obj_t *ui_Next_Prayer;
extern lv_obj_t *ui_Next_Prayer1;
extern lv_obj_t *ui_Next_Prayer_Remaining;
extern lv_obj_t *ui_Next_Prayer_Remaining1;
extern lv_obj_t *ui_Next_Prayer_Time;
extern lv_obj_t *ui_Next_Prayer_Time1;

// Add external reference to ui_Current_Date
extern lv_obj_t *ui_Current_Date;

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
    char remaining_time_str[16];

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

    // Format current date
    char date_str[32];
    // Arrays of abbreviated day and month names
    const char *day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    const char *month_names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    snprintf(date_str, sizeof(date_str), "%s\n%s %d",
             day_names[timeinfo.tm_wday],
             month_names[timeinfo.tm_mon],
             timeinfo.tm_mday);

    // Update the ui_Current_Date label
    lv_label_set_text(ui_Current_Date, date_str);

    // Setup prayer times array with names and UI elements
    struct {
        const char *name;
        lv_obj_t *container;
        lv_obj_t *time_label;
    } prayers[] = {
        {"Fajr", ui_Fajr_Container, ui_Fajr_Time},
        {"Sunrise", ui_Sunrise_Container, ui_Sunrise_Time},
        {"Duhur", ui_Duhur_Container, ui_Duhur_Time},
        {"Asr", ui_Asr_Container, ui_Asr_Time},
        {"Maghrib", ui_Maghrib_Container, ui_Maghrib_Time},
        {"Isha", ui_Isha_Container, ui_Isha_Time}
    };

    // Reset all containers to default color
    for (int i = 0; i < 6; i++) {
        lv_obj_set_style_bg_color(prayers[i].container, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(prayers[i].container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // Find next prayer time and previous prayer
    int next_prayer_idx = -1;
    int prev_prayer_idx = -1;
    int min_time_diff = 24 * 60; // Maximum minutes in a day
    const char* next_prayer_time = NULL;

    // Find the next prayer
    for (int i = 0; i < 6; i++) {
        const char* prayer_time = lv_label_get_text(prayers[i].time_label);
        int prayer_minutes = get_prayer_minutes(prayer_time);

        if (prayer_minutes >= 0) {
            int time_diff = prayer_minutes - current_minutes;
            if (time_diff < 0) time_diff += 24 * 60; // Adjust for next day

            if (time_diff < min_time_diff) {
                min_time_diff = time_diff;
                next_prayer_idx = i;
                next_prayer_time = prayer_time;
            }
        }
    }

    // Determine previous prayer
    if (next_prayer_idx >= 0) {
        prev_prayer_idx = (next_prayer_idx - 1 + 6) % 6;

        // Get previous and next prayer times in minutes
        const char* prev_prayer_time_str = lv_label_get_text(prayers[prev_prayer_idx].time_label);
        int prev_prayer_minutes = get_prayer_minutes(prev_prayer_time_str);
        int next_prayer_minutes = get_prayer_minutes(next_prayer_time);

        // Calculate time window between prayers
        int time_window = next_prayer_minutes - prev_prayer_minutes;
        if (time_window <= 0) time_window += 24 * 60; // Adjust for next day

        // Calculate gradient stop value using dynamic time window
        int stop_value = 255 - ((min_time_diff * 255) / time_window);
        if (stop_value < 0) stop_value = 0;
        if (stop_value > 255) stop_value = 255;

        // Update gradient stop for both panels
        lv_obj_set_style_bg_main_stop(ui_Next_Prayer_Panel, stop_value, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_main_stop(ui_Next_Prayer_Panel1, stop_value, LV_PART_MAIN | LV_STATE_DEFAULT);

        ESP_LOGI(TAG, "Prayer window: %s (%d min) -> %s (%d min), window: %d minutes", 
                prayers[prev_prayer_idx].name, prev_prayer_minutes,
                prayers[next_prayer_idx].name, next_prayer_minutes,
                time_window);

        // Format remaining time
        int hours = min_time_diff / 60;
        int minutes = min_time_diff % 60;
        snprintf(remaining_time_str, sizeof(remaining_time_str), "%d:%02d", hours, minutes);

        // Set color based on remaining time
        lv_color_t remaining_time_color = (min_time_diff > 60) ? 
            lv_color_hex(0x00FF37) :  // Green for > 1 hour
            lv_color_hex(0xFF0000);   // Red for <= 1 hour

        // Update all next prayer related labels
        lv_label_set_text(ui_Next_Prayer, prayers[next_prayer_idx].name);
        lv_label_set_text(ui_Next_Prayer1, prayers[next_prayer_idx].name);
        
        // Update container highlight and UI elements
        lv_obj_set_style_bg_color(prayers[next_prayer_idx].container, lv_color_hex(0xFFD29C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(prayers[next_prayer_idx].container, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

        // Update remaining time labels with color
        lv_label_set_text(ui_Next_Prayer_Remaining, remaining_time_str);
        lv_label_set_text(ui_Next_Prayer_Remaining1, remaining_time_str);
        lv_obj_set_style_text_color(ui_Next_Prayer_Remaining, remaining_time_color, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui_Next_Prayer_Remaining1, remaining_time_color, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        lv_label_set_text(ui_Next_Prayer_Time, next_prayer_time);
        lv_label_set_text(ui_Next_Prayer_Time1, next_prayer_time);

        ESP_LOGI(TAG, "Next prayer: %s in %s (%s) [Color: %s, Gradient: %d]", 
                prayers[next_prayer_idx].name, 
                remaining_time_str, 
                next_prayer_time,
                min_time_diff > 60 ? "Green" : "Red",
                stop_value);
    }
    ESP_LOGI(TAG, "Time updated: %s", time_str);
    if (!is_clock_initialized()) {
        set_clock_initialized();
        lv_scr_load(ui_Main_Screen);
    }
}

// Task to update the LVGL label every minute
static void time_update_task(void *arg) {
    if (!is_clock_initialized()) {
        take_ui_mutex("time_update_task");
        lv_label_set_text(ui_Loading_Status_Text, "Synchronizing Clock...");
        give_ui_mutex("time_update_task");    
    }

    while (true) {
        take_ui_mutex("time_update_task");
        update_time_ui();
        give_ui_mutex("time_update_task");
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000)); // Wait for notification or 1 minute
    }
}

// Add notification function
void notify_clock(void) {
    if (time_update_task_handle != NULL) {
        xTaskNotifyGive(time_update_task_handle);
        ESP_LOGI(TAG, "Notification sent to clock update task");
    }
}

// Function to initialize the clock module
void clock_init(void) {
    if (is_clock_initialized()) {
        ESP_LOGI(TAG, "Clock already initialized, skipping...");
        return;
    }

    xTaskCreate(time_update_task, "time_update_task", 4096, NULL, 5, &time_update_task_handle);
    ESP_LOGI(TAG, "Clock initialized successfully");
}
