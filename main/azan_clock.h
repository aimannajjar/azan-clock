#include "lvgl/lvgl.h"

#ifndef _AZAN_CLOCK_
#define _AZAN_CLOCK_

void azan_clock(void);
void update_status_text(const char *text);
void set_system_initialized();
bool is_system_initialized();

void take_ui_mutex(const char *caller);
void give_ui_mutex(const char *caller);

#endif