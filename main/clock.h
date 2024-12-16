#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>
#include "lvgl/lvgl.h"

extern lv_obj_t *ui_Current_Time;

// Initialize the clock module
void clock_init(void);

// Start the NTP synchronization task
void start_ntp_sync_task(void);

// Start the time update task
void start_time_update_task(void);

// Set the timezone (default is EST)
void clock_set_timezone(const char *timezone);

#endif // CLOCK_H
