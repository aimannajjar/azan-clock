// wifi.h
#ifndef WIFI_H
#define WIFI_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "lvgl/lvgl.h"
#include "esp_wifi.h"
#include "esp_log.h"

void wifi_init();
void wifi_scan_task(void *param);
void lvgl_task(void *param);
void stop_scan_task(lv_event_t *e);
void start_scan_task(lv_event_t *e);
void connect_wifi();

#endif // WIFI_H
