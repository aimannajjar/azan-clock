#ifndef _AZAN_CLOCK_
#define _AZAN_CLOCK_

#define lv_screen_active() lv_scr_act()
#define lv_button_create(x) lv_btn_create(x)

void azan_clock(void);
void entry(void);
static void btn_event_cb(lv_event_t*);

#endif