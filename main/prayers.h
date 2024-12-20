#ifndef PRAYERS_H
#define PRAYERS_H

#include "esp_system.h"

void prayers_init(void);
esp_err_t get_prayer_times(float latitude, float longitude);
void notify_prayers(void);

#endif // PRAYERS_H
