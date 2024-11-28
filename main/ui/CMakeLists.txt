SET(SOURCES screens/ui_Main_Screen.c
    screens/ui_Screen_2_Move.c
    screens/ui_Screen_3_Setting.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_minaret_bg_png.c
    images/ui_img_icn_wifi_png.c
    images/ui_img_icn_pc_png.c
    images/ui_img_icn_usb_png.c
    images/ui_img_icn_time_2_png.c
    images/ui_img_rainy_weather_png.c
    images/ui_img_3d_printer_bg_png.c
    images/ui_img_btn_print_png.c
    images/ui_img_btn_move_png.c
    images/ui_img_btn_setting_png.c
    images/ui_img_btn_z_botton_png.c
    images/ui_img_btn_z_top_png.c
    images/ui_img_arrow_up_png.c
    images/ui_img_arrow_down_png.c
    images/ui_img_btn_pad_bottom_png.c
    images/ui_img_btn_pad_top_png.c
    images/ui_img_arrow_left_png.c
    images/ui_img_arrow_right_png.c
    images/ui_img_roller_bg_png.c
    images/ui_img_btn_main_off_png.c
    images/ui_img_btn_main_on_png.c
    images/ui_img_icn_time_1_png.c
    fonts/ui_font_Small_Font.c
    fonts/ui_font_FontAwesome_Regular_24.c
    fonts/ui_font_FontAwesome_Brands_24.c
    fonts/ui_font_FontAwesome_Solid_24_1.c
    fonts/ui_font_FontAwesome_Solid_24_2.c
    fonts/ui_font_Montserrat_SemiBold_60.c
    fonts/ui_font_Montserrat_SemiBold_80.c)

add_library(ui ${SOURCES})
