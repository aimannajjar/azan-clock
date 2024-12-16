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
extern lv_obj_t *ui_Main_Screen;
extern lv_obj_t *ui_Setup_Screen;
extern QueueHandle_t ui_update_queue;

void wifi_scan_task(void *param);
void lvgl_task(void *param);
void wifi_init();
void start_scan_task();
void connect_wifi();
esp_err_t load_connection_params(char *ssid, size_t ssid_size, char *password, size_t password_size);

#endif // WIFI_H
