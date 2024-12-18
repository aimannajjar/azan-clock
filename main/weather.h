#include "lvgl/lvgl.h"
#include "esp_system.h"

#ifndef WEATHER_H
#define WEATHER_H

void weather_init();
esp_err_t get_weather_forecast(float latitude, float longitude);

// Function to get the image resource for a given weather code
const lv_img_dsc_t* get_weather_image(int is_day, int weather_code);

#endif // WEATHER_H
