#ifndef SETTINGS_H
#define SETTINGS_H

#include "esp_err.h"
#include "lvgl/lvgl.h"

void get_user_location(lv_event_t *e);
void keypad_ready(lv_event_t *e);
void settings_init(void);
esp_err_t load_settings(void);
esp_err_t save_settings(void);

#endif // SETTINGS_H