// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: azan_clock

#include "../ui.h"

void ui_Setup_Screen_screen_init(void)
{
    ui_Setup_Screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(ui_Setup_Screen, lv_color_hex(0x191D26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Setup_Screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Background1 = lv_img_create(ui_Setup_Screen);
    lv_img_set_src(ui_Background1, &ui_img_minaret_bg_png);
    lv_obj_set_width(ui_Background1, 800);
    lv_obj_set_height(ui_Background1, 480);
    lv_obj_set_align(ui_Background1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Background1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags

    ui_Panel_Header1 = lv_obj_create(ui_Setup_Screen);
    lv_obj_set_width(ui_Panel_Header1, 665);
    lv_obj_set_height(ui_Panel_Header1, 39);
    lv_obj_set_x(ui_Panel_Header1, -30);
    lv_obj_set_y(ui_Panel_Header1, 0);
    lv_obj_set_align(ui_Panel_Header1, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(ui_Panel_Header1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel_Header1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel_Header1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Panel_Header1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Panel_Header1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel_Header1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IMG_Wifi1 = lv_img_create(ui_Panel_Header1);
    lv_img_set_src(ui_IMG_Wifi1, &ui_img_icn_wifi_png);
    lv_obj_set_width(ui_IMG_Wifi1, LV_SIZE_CONTENT);   /// 100
    lv_obj_set_height(ui_IMG_Wifi1, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_align(ui_IMG_Wifi1, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_IMG_Wifi1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags

    ui_IMG_PC1 = lv_img_create(ui_Panel_Header1);
    lv_img_set_src(ui_IMG_PC1, &ui_img_icn_pc_png);
    lv_obj_set_width(ui_IMG_PC1, LV_SIZE_CONTENT);   /// 100
    lv_obj_set_height(ui_IMG_PC1, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_IMG_PC1, -42);
    lv_obj_set_y(ui_IMG_PC1, 0);
    lv_obj_set_align(ui_IMG_PC1, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_IMG_PC1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags

    ui_IMG_USB1 = lv_img_create(ui_Panel_Header1);
    lv_img_set_src(ui_IMG_USB1, &ui_img_icn_usb_png);
    lv_obj_set_width(ui_IMG_USB1, LV_SIZE_CONTENT);   /// 100
    lv_obj_set_height(ui_IMG_USB1, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_IMG_USB1, -84);
    lv_obj_set_y(ui_IMG_USB1, 0);
    lv_obj_set_align(ui_IMG_USB1, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_IMG_USB1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags

    ui_Panel8 = lv_obj_create(ui_Setup_Screen);
    lv_obj_set_width(ui_Panel8, 70);
    lv_obj_set_height(ui_Panel8, 480);
    lv_obj_set_align(ui_Panel8, LV_ALIGN_LEFT_MID);
    lv_obj_set_flex_flow(ui_Panel8, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel8, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button4 = lv_btn_create(ui_Panel8);
    lv_obj_set_width(ui_Button4, 50);
    lv_obj_set_height(ui_Button4, 50);
    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button4, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button4, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button4, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Button4, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Button4, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Button4, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Button4, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(ui_Button4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui_Button4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label16 = lv_label_create(ui_Button4);
    lv_obj_set_width(ui_Label16, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label16, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label16, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label16, "");
    lv_obj_set_style_text_font(ui_Label16, &ui_font_FontAwesome_Regular_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button5 = lv_btn_create(ui_Panel8);
    lv_obj_set_width(ui_Button5, 50);
    lv_obj_set_height(ui_Button5, 50);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button5, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button5, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button5, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button5, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button5, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button5, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(ui_Button5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui_Button5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(ui_Button5, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui_Button5, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(ui_Button5, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button5, 100, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui_Button5, 40, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(ui_Button5, 5, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_Label17 = lv_label_create(ui_Button5);
    lv_obj_set_width(ui_Label17, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label17, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label17, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label17, "");
    lv_obj_set_style_text_font(ui_Label17, &ui_font_FontAwesome_Solid_24_2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button6 = lv_btn_create(ui_Panel8);
    lv_obj_set_width(ui_Button6, 50);
    lv_obj_set_height(ui_Button6, 50);
    lv_obj_set_align(ui_Button6, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Button6, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Button6, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Button6, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Button6, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Button6, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button6, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button6, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button6, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_Button6, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(ui_Button6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(ui_Button6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(ui_Button6, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui_Button6, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(ui_Button6, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button6, 100, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui_Button6, 40, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(ui_Button6, 5, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_Label18 = lv_label_create(ui_Button6);
    lv_obj_set_width(ui_Label18, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label18, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label18, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label18, "");
    lv_obj_set_style_text_font(ui_Label18, &ui_font_FontAwesome_Solid_24_1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WiFi_Networks = lv_dropdown_create(ui_Setup_Screen);
    lv_dropdown_set_options(ui_WiFi_Networks, "Scanning Networks");
    lv_obj_set_width(ui_WiFi_Networks, 386);
    lv_obj_set_height(ui_WiFi_Networks, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_WiFi_Networks, 19);
    lv_obj_set_y(ui_WiFi_Networks, -110);
    lv_obj_set_align(ui_WiFi_Networks, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_WiFi_Networks, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags



    ui_WiFi_Password = lv_textarea_create(ui_Setup_Screen);
    lv_obj_set_width(ui_WiFi_Password, 387);
    lv_obj_set_height(ui_WiFi_Password, LV_SIZE_CONTENT);    /// 70
    lv_obj_set_x(ui_WiFi_Password, 20);
    lv_obj_set_y(ui_WiFi_Password, -53);
    lv_obj_set_align(ui_WiFi_Password, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_WiFi_Password, "Placeholder...");
    lv_textarea_set_one_line(ui_WiFi_Password, true);
    lv_textarea_set_password_mode(ui_WiFi_Password, true);



    ui_Spinner1 = lv_spinner_create(ui_Setup_Screen, 1000, 90);
    lv_obj_set_width(ui_Spinner1, 44);
    lv_obj_set_height(ui_Spinner1, 38);
    lv_obj_set_x(ui_Spinner1, 181);
    lv_obj_set_y(ui_Spinner1, 89);
    lv_obj_set_align(ui_Spinner1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Spinner1, LV_OBJ_FLAG_CLICKABLE);      /// Flags

    ui_ConnectButton = lv_btn_create(ui_Setup_Screen);
    lv_obj_set_width(ui_ConnectButton, 120);
    lv_obj_set_height(ui_ConnectButton, 50);
    lv_obj_set_x(ui_ConnectButton, 302);
    lv_obj_set_y(ui_ConnectButton, -76);
    lv_obj_set_align(ui_ConnectButton, LV_ALIGN_CENTER);

    ui_Label10 = lv_label_create(ui_ConnectButton);
    lv_obj_set_width(ui_Label10, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label10, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label10, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label10, "Connect");

    ui_Label11 = lv_label_create(ui_Setup_Screen);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label11, 12);
    lv_obj_set_y(ui_Label11, -156);
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "Setup Wi-Fi");
    lv_obj_set_style_text_color(ui_Label11, lv_color_hex(0x925E08), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label11, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label11, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Keyboard1 = lv_keyboard_create(ui_Setup_Screen);
    lv_obj_set_width(ui_Keyboard1, 683);
    lv_obj_set_height(ui_Keyboard1, 233);
    lv_obj_set_x(ui_Keyboard1, 42);
    lv_obj_set_y(ui_Keyboard1, 100);
    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_Label10, ui_event_Label10, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ConnectButton, ui_event_ConnectButton, LV_EVENT_ALL, NULL);
    lv_keyboard_set_textarea(ui_Keyboard1, ui_WiFi_Password);

}