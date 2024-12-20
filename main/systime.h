#ifndef SYSTIME_H
#define SYSTIME_H

void systime_init(void);
void systime_task(void *pvParameters);
void notify_systime(void);

#endif // SYSTIME_H