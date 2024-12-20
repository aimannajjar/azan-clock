#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern lv_obj_t *ui_Current_Time;
extern TaskHandle_t time_update_task_handle; // Add this line

// Initialize the clock module
void clock_init(void);

// Start the NTP synchronization task
void start_ntp_sync_task(void);

// Start the time update task
void start_time_update_task(void);

// Set the timezone (default is EST)
void clock_set_timezone(const char *timezone);

void notify_clock(void);

#endif // CLOCK_H