// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: azan_clock

#ifndef _AZAN_CLOCK_UI_H
#define _AZAN_CLOCK_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"

void OpaOn_Animation(lv_obj_t * TargetObject, int delay);
void OpaOff_Animation(lv_obj_t * TargetObject, int delay);
void ScreenIn_Animation(lv_obj_t * TargetObject, int delay);
void MenuIn_Animation(lv_obj_t * TargetObject, int delay);
void MenuOut_Animation(lv_obj_t * TargetObject, int delay);
void ScreenOut_Animation(lv_obj_t * TargetObject, int delay);

// SCREEN: ui_Loading_Screen
void ui_Loading_Screen_screen_init(void);
extern lv_obj_t * ui_Loading_Screen;
extern lv_obj_t * ui_Background2;
extern lv_obj_t * ui_Spinner2;
extern lv_obj_t * ui_Loading_Status_Text;
// CUSTOM VARIABLES

// SCREEN: ui_Main_Screen
void ui_Main_Screen_screen_init(void);
extern lv_obj_t * ui_Main_Screen;
extern lv_obj_t * ui_Background;
extern lv_obj_t * ui_S1_Content_Panel;
extern lv_obj_t * ui_Next_Prayer_Panel;
extern lv_obj_t * ui_Next_Prayer;
extern lv_obj_t * ui_Next_Prayer_Remaining;
extern lv_obj_t * ui_Next_Prayer_Time;
extern lv_obj_t * ui_Current_Time;
extern lv_obj_t * ui_Weather_Condition_Panel;
extern lv_obj_t * ui_Weather_Image;
extern lv_obj_t * ui_Weather_Condition_Panel1;
extern lv_obj_t * ui_Current_Date;
extern lv_obj_t * ui_Left_Panel_Main;
// CUSTOM VARIABLES

// SCREEN: ui_Setup_Screen
void ui_Setup_Screen_screen_init(void);
void ui_event_Setup_Screen(lv_event_t * e);
extern lv_obj_t * ui_Setup_Screen;
void ui_event_Background1(lv_event_t * e);
extern lv_obj_t * ui_Background1;
extern lv_obj_t * ui_Panel_Header1;
extern lv_obj_t * ui_WiFi_Networks;
void ui_event_WiFi_Password(lv_event_t * e);
extern lv_obj_t * ui_WiFi_Password;
void ui_event_ConnectButton(lv_event_t * e);
extern lv_obj_t * ui_ConnectButton;
void ui_event_Label10(lv_event_t * e);
extern lv_obj_t * ui_Label10;
extern lv_obj_t * ui_Label11;
extern lv_obj_t * ui_Left_Panel_Setup;
void ui_event_Keyboard1(lv_event_t * e);
extern lv_obj_t * ui_Keyboard1;
extern lv_obj_t * ui_Connecting_Modal;
extern lv_obj_t * ui_Spinner1;
extern lv_obj_t * ui_Connection_Failed_Label;
// CUSTOM VARIABLES

// SCREEN: ui_Prayers_Screen
void ui_Prayers_Screen_screen_init(void);
extern lv_obj_t * ui_Prayers_Screen;
extern lv_obj_t * ui_Background3;
extern lv_obj_t * ui_S1_Content_Panel1;
extern lv_obj_t * ui_Next_Prayer_Panel1;
extern lv_obj_t * ui_Next_Prayer1;
extern lv_obj_t * ui_Next_Prayer_Remaining1;
extern lv_obj_t * ui_Next_Prayer_Time1;
extern lv_obj_t * ui_Date_Panel1;
extern lv_obj_t * ui_DT1;
extern lv_obj_t * ui_Label_Head_Temp1;
extern lv_obj_t * ui_Label_Printing_Head_Temp_1;
extern lv_obj_t * ui_Panel3;
extern lv_obj_t * ui_DT3_copy1;
extern lv_obj_t * ui_Label_Bed_Temp1;
extern lv_obj_t * ui_Label_Printing_Bed_Temp2;
extern lv_obj_t * ui_Label2;
extern lv_obj_t * ui_Current_Time1;
extern lv_obj_t * ui_Prayers_List_Panel;
extern lv_obj_t * ui_Fajr_Container;
extern lv_obj_t * ui_Fajr_Label;
extern lv_obj_t * ui_Fajr_Time;
extern lv_obj_t * ui_Sunrise_Container;
extern lv_obj_t * ui_Sunrise_Label;
extern lv_obj_t * ui_Sunrise_Time;
extern lv_obj_t * ui_Duhur_Container;
extern lv_obj_t * ui_Duhur_Label;
extern lv_obj_t * ui_Duhur_Time;
extern lv_obj_t * ui_Asr_Container;
extern lv_obj_t * ui_Asr_Label;
extern lv_obj_t * ui_Asr_Time;
extern lv_obj_t * ui_Maghrib_Container;
extern lv_obj_t * ui_Maghrib_Label;
extern lv_obj_t * ui_Maghrib_Time;
extern lv_obj_t * ui_Isha_Container;
extern lv_obj_t * ui_Isha_Label;
extern lv_obj_t * ui_Isha_Time;
extern lv_obj_t * ui_Left_Panel_Prayers;
// CUSTOM VARIABLES

// SCREEN: ui_Settings_Screen
void ui_Settings_Screen_screen_init(void);
extern lv_obj_t * ui_Settings_Screen;
extern lv_obj_t * ui_Background4;
void ui_event_Save_Button(lv_event_t * e);
extern lv_obj_t * ui_Save_Button;
extern lv_obj_t * ui_Label24;
extern lv_obj_t * ui_Settings_Content_Panel;
extern lv_obj_t * ui_Location_Settings_Container;
extern lv_obj_t * ui_Location_Label;
void ui_event_Latitude(lv_event_t * e);
extern lv_obj_t * ui_Latitude;
extern lv_obj_t * ui_Spacer1;
void ui_event_Longitude(lv_event_t * e);
extern lv_obj_t * ui_Longitude;
extern lv_obj_t * ui_Spacer;
void ui_event_Locate_Me(lv_event_t * e);
extern lv_obj_t * ui_Locate_Me;
extern lv_obj_t * ui_Label23;
extern lv_obj_t * ui_Location_Name;
extern lv_obj_t * ui_Method_Setting_Container;
extern lv_obj_t * ui_Timezone_Label;
extern lv_obj_t * ui_Timezone_Dropdown;
extern lv_obj_t * ui_Method_Setting_Container1;
extern lv_obj_t * ui_Location_Label2;
extern lv_obj_t * ui_Calculation_Method_Dropdown;
extern lv_obj_t * ui_Label25;
extern lv_obj_t * ui_Label26;
extern lv_obj_t * ui_LocatorQR;
void ui_event_Keypad(lv_event_t * e);
extern lv_obj_t * ui_Keypad;
extern lv_obj_t * ui_Left_Panel_Settings;
extern lv_obj_t * ui_No_Wifi_Settings;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Label1;
// CUSTOM VARIABLES

// EVENTS
extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_minaret_bg_png);    // assets/minaret_bg.png
LV_IMG_DECLARE(ui_img_no_wifi_png);    // assets/no_wifi.png
LV_IMG_DECLARE(ui_img_locatorqr_png);    // assets/locatorqr.png
LV_IMG_DECLARE(ui_img_no_wifi_large_png);    // assets/no_wifi_large.png
LV_IMG_DECLARE(ui_img_day_clear_png);    // assets/day_clear.png
LV_IMG_DECLARE(ui_img_day_fog_png);    // assets/day_fog.png
LV_IMG_DECLARE(ui_img_day_heavy_rain_png);    // assets/day_heavy_rain.png
LV_IMG_DECLARE(ui_img_day_heavy_snowfall_png);    // assets/day_heavy_snowfall.png
LV_IMG_DECLARE(ui_img_day_light_drizzle_png);    // assets/day_light_drizzle.png
LV_IMG_DECLARE(ui_img_day_light_rain_png);    // assets/day_light_rain.png
LV_IMG_DECLARE(ui_img_day_moderate_rain_png);    // assets/day_moderate_rain.png
LV_IMG_DECLARE(ui_img_day_overcast_png);    // assets/day_overcast.png
LV_IMG_DECLARE(ui_img_day_partly_cloudy_png);    // assets/day_partly_cloudy.png
LV_IMG_DECLARE(ui_img_day_rime_fog_png);    // assets/day_rime_fog.png
LV_IMG_DECLARE(ui_img_day_snowflake_png);    // assets/day_snowflake.png
LV_IMG_DECLARE(ui_img_day_thunderstorm_png);    // assets/day_thunderstorm.png
LV_IMG_DECLARE(ui_img_icn_bed_png);    // assets/icn_bed.png
LV_IMG_DECLARE(ui_img_icn_stop_png);    // assets/icn_stop.png
LV_IMG_DECLARE(ui_img_night_clear_png);    // assets/night_clear.png
LV_IMG_DECLARE(ui_img_night_fog_png);    // assets/night_fog.png
LV_IMG_DECLARE(ui_img_night_heavy_rain_png);    // assets/night_heavy_rain.png
LV_IMG_DECLARE(ui_img_night_heavy_snowfall_png);    // assets/night_heavy_snowfall.png
LV_IMG_DECLARE(ui_img_night_light_drizzle_png);    // assets/night_light_drizzle.png
LV_IMG_DECLARE(ui_img_night_light_rain_png);    // assets/night_light_rain.png
LV_IMG_DECLARE(ui_img_night_moderate_rain_png);    // assets/night_moderate_rain.png
LV_IMG_DECLARE(ui_img_night_overcast_png);    // assets/night_overcast.png
LV_IMG_DECLARE(ui_img_night_partly_cloudy_png);    // assets/night_partly_cloudy.png
LV_IMG_DECLARE(ui_img_night_snowflake_png);    // assets/night_snowflake.png
LV_IMG_DECLARE(ui_img_night_thunderstorm_png);    // assets/night_thunderstorm.png
LV_IMG_DECLARE(ui_img_rainy_weather_png);    // assets/rainy_weather.png

// FONTS
LV_FONT_DECLARE(ui_font_FontAwesome_Brands_24);
LV_FONT_DECLARE(ui_font_FontAwesome_Regular_24);
LV_FONT_DECLARE(ui_font_FontAwesome_Solid_24_1);
LV_FONT_DECLARE(ui_font_FontAwesome_Solid_24_2);
LV_FONT_DECLARE(ui_font_Montserrat_SemiBold_110);
LV_FONT_DECLARE(ui_font_Montserrat_SemiBold_60);
LV_FONT_DECLARE(ui_font_Montserrat_SemiBold_80);
LV_FONT_DECLARE(ui_font_Montserrat_SemiBold_90);
LV_FONT_DECLARE(ui_font_Small_Font);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
