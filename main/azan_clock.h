#include "lvgl/lvgl.h"

#ifndef _AZAN_CLOCK_
#define _AZAN_CLOCK_

void azan_clock(void);
void update_status_text(const char *text);
void set_wifi_initialized();
bool is_wifi_initialized();
void set_clock_initialized();
bool is_clock_initialized();
void set_weather_initialized();
bool is_weather_initialized();
void set_systime_initialized();
bool is_settings_initialized();
void set_settings_initialized();
bool is_systime_initialized();
bool is_prayers_initialized(void);
void set_prayers_initialized(void);

void take_ui_mutex(const char *caller);
void give_ui_mutex(const char *caller);

void set_current_latitude(float lat);
void set_current_longitude(float lon);
void set_current_city(const char* city);
void set_current_timezone(uint16_t tz);
void set_current_calculation_method(uint8_t method);

float get_current_latitude(void);
float get_current_longitude(void);
const char* get_current_city(void);
uint16_t get_current_timezone(void);
uint8_t get_current_calculation_method(void);

void change_screen(lv_event_t *t);

#endif