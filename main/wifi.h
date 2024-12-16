// wifi.h
#ifndef WIFI_H
#define WIFI_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lvgl/lvgl.h"
#include "esp_wifi.h"
#include "esp_log.h"

extern lv_obj_t *ui_WiFi_Networks;
extern lv_obj_t *ui_WiFi_Password;
extern QueueHandle_t ui_update_queue;

void wifi_scan_task(void *param);
void lvgl_task(void *param);
void wifi_init();
void connect_wifi();

#endif // WIFI_H
