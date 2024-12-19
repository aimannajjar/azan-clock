#include "lvgl/lvgl.h"

#ifndef _AZAN_CLOCK_
#define _AZAN_CLOCK_

void azan_clock(void);
void update_status_text(const char *text);
void set_wifi_previously_connected();
bool is_wifi_previously_connected();
void set_clock_initialized();
bool is_clock_initialized();
void set_weather_initialized();
bool is_weather_initialized();
bool is_prayers_initialized(void);
void set_prayers_initialized(void);

void take_ui_mutex(const char *caller);
void give_ui_mutex(const char *caller);

#endif