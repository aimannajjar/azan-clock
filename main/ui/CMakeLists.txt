SET(SOURCES screens/ui_Main_Screen.c
    screens/ui_Setup_Screen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_minaret_bg_png.c
    images/ui_img_icn_wifi_png.c
    images/ui_img_icn_pc_png.c
    images/ui_img_icn_usb_png.c
    images/ui_img_icn_time_2_png.c
    images/ui_img_rainy_weather_png.c
    images/ui_img_icn_time_1_png.c
    fonts/ui_font_FontAwesome_Brands_24.c
    fonts/ui_font_FontAwesome_Regular_24.c
    fonts/ui_font_FontAwesome_Solid_24_1.c
    fonts/ui_font_FontAwesome_Solid_24_2.c
    fonts/ui_font_Montserrat_SemiBold_60.c
    fonts/ui_font_Montserrat_SemiBold_80.c
    fonts/ui_font_Small_Font.c)

add_library(ui ${SOURCES})
