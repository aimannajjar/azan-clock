// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: azan_clock

#include "../ui.h"


void ui_event_comp_LeftPanel_Main_Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t ** comp_LeftPanel = lv_event_get_user_data(e);

    if(event_code == LV_EVENT_CLICKED) {
        change_screen(e);
    }
}

void ui_event_comp_LeftPanel_Prayers_Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t ** comp_LeftPanel = lv_event_get_user_data(e);

    if(event_code == LV_EVENT_CLICKED) {
        change_screen(e);
    }
}

void ui_event_comp_LeftPanel_Setup_Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t ** comp_LeftPanel = lv_event_get_user_data(e);

    if(event_code == LV_EVENT_CLICKED) {
        start_scan_task(e);
        change_screen(e);
    }
}

void ui_event_comp_LeftPanel_Settings_Button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t ** comp_LeftPanel = lv_event_get_user_data(e);

    if(event_code == LV_EVENT_CLICKED) {
        change_screen(e);
    }
}

// COMPONENT LeftPanel

lv_obj_t * ui_LeftPanel_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_LeftPanel;
    cui_LeftPanel = lv_obj_create(comp_parent);
    lv_obj_set_width(cui_LeftPanel, 70);
    lv_obj_set_height(cui_LeftPanel, 480);
    lv_obj_set_align(cui_LeftPanel, LV_ALIGN_LEFT_MID);
    lv_obj_set_flex_flow(cui_LeftPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cui_LeftPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(cui_LeftPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(cui_LeftPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_LeftPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_Main_Button;
    cui_Main_Button = lv_btn_create(cui_LeftPanel);
    lv_obj_set_width(cui_Main_Button, 50);
    lv_obj_set_height(cui_Main_Button, 50);
    lv_obj_set_align(cui_Main_Button, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cui_Main_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Main_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(cui_Main_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(cui_Main_Button, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(cui_Main_Button, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(cui_Main_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Main_Button, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Main_Button, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Main_Button, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(cui_Main_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_Main_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Main_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(cui_Main_Button, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Main_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(cui_Main_Button, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Main_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(cui_Main_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(cui_Main_Button, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(cui_Main_Button, 100, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(cui_Main_Button, 40, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(cui_Main_Button, 5, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t * cui_Label4;
    cui_Label4 = lv_label_create(cui_Main_Button);
    lv_obj_set_width(cui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(cui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label4, "");
    lv_obj_set_style_text_font(cui_Label4, &ui_font_FontAwesome_Regular_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_Prayers_Button;
    cui_Prayers_Button = lv_btn_create(cui_LeftPanel);
    lv_obj_set_width(cui_Prayers_Button, 50);
    lv_obj_set_height(cui_Prayers_Button, 50);
    lv_obj_set_align(cui_Prayers_Button, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cui_Prayers_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Prayers_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(cui_Prayers_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(cui_Prayers_Button, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(cui_Prayers_Button, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(cui_Prayers_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Prayers_Button, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Prayers_Button, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Prayers_Button, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(cui_Prayers_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_Prayers_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Prayers_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(cui_Prayers_Button, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Prayers_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(cui_Prayers_Button, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Prayers_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(cui_Prayers_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(cui_Prayers_Button, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(cui_Prayers_Button, 100, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(cui_Prayers_Button, 40, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(cui_Prayers_Button, 5, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t * cui_Label6;
    cui_Label6 = lv_label_create(cui_Prayers_Button);
    lv_obj_set_width(cui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(cui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label6, "");
    lv_obj_set_style_text_font(cui_Label6, &ui_font_FontAwesome_Solid_24_1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_Setup_Button;
    cui_Setup_Button = lv_btn_create(cui_LeftPanel);
    lv_obj_set_width(cui_Setup_Button, 50);
    lv_obj_set_height(cui_Setup_Button, 50);
    lv_obj_set_align(cui_Setup_Button, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cui_Setup_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Setup_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(cui_Setup_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(cui_Setup_Button, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(cui_Setup_Button, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(cui_Setup_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Setup_Button, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Setup_Button, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Setup_Button, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(cui_Setup_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_Setup_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Setup_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(cui_Setup_Button, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Setup_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(cui_Setup_Button, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Setup_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(cui_Setup_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(cui_Setup_Button, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(cui_Setup_Button, 100, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(cui_Setup_Button, 40, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(cui_Setup_Button, 5, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t * cui_Label5;
    cui_Label5 = lv_label_create(cui_Setup_Button);
    lv_obj_set_width(cui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(cui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label5, "");
    lv_obj_set_style_text_font(cui_Label5, &ui_font_FontAwesome_Solid_24_2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_Settings_Button;
    cui_Settings_Button = lv_btn_create(cui_LeftPanel);
    lv_obj_set_width(cui_Settings_Button, 50);
    lv_obj_set_height(cui_Settings_Button, 50);
    lv_obj_set_align(cui_Settings_Button, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cui_Settings_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Settings_Button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(cui_Settings_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(cui_Settings_Button, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(cui_Settings_Button, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(cui_Settings_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Settings_Button, lv_color_hex(0xFFEDAD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Settings_Button, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Settings_Button, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(cui_Settings_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(cui_Settings_Button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Settings_Button, lv_color_hex(0xEFA337), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(cui_Settings_Button, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Settings_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_dir(cui_Settings_Button, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_grad_color(cui_Settings_Button, lv_color_hex(0xFFBC58), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(cui_Settings_Button, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(cui_Settings_Button, lv_color_hex(0xFFF2C7), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(cui_Settings_Button, 100, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(cui_Settings_Button, 40, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(cui_Settings_Button, 5, LV_PART_MAIN | LV_STATE_FOCUSED);

    lv_obj_t * cui_Label3;
    cui_Label3 = lv_label_create(cui_Settings_Button);
    lv_obj_set_width(cui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(cui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(cui_Label3, "");
    lv_obj_set_style_text_font(cui_Label3, &ui_font_FontAwesome_Solid_24_1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_WiFi_Disconnected;
    cui_WiFi_Disconnected = lv_obj_create(cui_LeftPanel);
    lv_obj_remove_style_all(cui_WiFi_Disconnected);
    lv_obj_set_width(cui_WiFi_Disconnected, 50);
    lv_obj_set_height(cui_WiFi_Disconnected, 50);
    lv_obj_set_x(cui_WiFi_Disconnected, -369);
    lv_obj_set_y(cui_WiFi_Disconnected, 70);
    lv_obj_set_align(cui_WiFi_Disconnected, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_WiFi_Disconnected, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_clear_flag(cui_WiFi_Disconnected, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t * cui_Image6;
    cui_Image6 = lv_img_create(cui_WiFi_Disconnected);
    lv_img_set_src(cui_Image6, &ui_img_no_wifi_png);
    lv_obj_set_width(cui_Image6, 32);
    lv_obj_set_height(cui_Image6, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_align(cui_Image6, LV_ALIGN_CENTER);
    lv_obj_add_flag(cui_Image6, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(cui_Image6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(cui_Image6, 255);

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_LEFTPANEL_NUM);
    children[UI_COMP_LEFTPANEL_LEFTPANEL] = cui_LeftPanel;
    children[UI_COMP_LEFTPANEL_MAIN_BUTTON] = cui_Main_Button;
    children[UI_COMP_LEFTPANEL_MAIN_BUTTON_LABEL4] = cui_Label4;
    children[UI_COMP_LEFTPANEL_PRAYERS_BUTTON] = cui_Prayers_Button;
    children[UI_COMP_LEFTPANEL_PRAYERS_BUTTON_LABEL6] = cui_Label6;
    children[UI_COMP_LEFTPANEL_SETUP_BUTTON] = cui_Setup_Button;
    children[UI_COMP_LEFTPANEL_SETUP_BUTTON_LABEL5] = cui_Label5;
    children[UI_COMP_LEFTPANEL_SETTINGS_BUTTON] = cui_Settings_Button;
    children[UI_COMP_LEFTPANEL_SETTINGS_BUTTON_LABEL3] = cui_Label3;
    children[UI_COMP_LEFTPANEL_WIFI_DISCONNECTED] = cui_WiFi_Disconnected;
    children[UI_COMP_LEFTPANEL_WIFI_DISCONNECTED_IMAGE6] = cui_Image6;
    lv_obj_add_event_cb(cui_LeftPanel, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_LeftPanel, del_component_child_event_cb, LV_EVENT_DELETE, children);
    lv_obj_add_event_cb(cui_Main_Button, ui_event_comp_LeftPanel_Main_Button, LV_EVENT_ALL, children);
    lv_obj_add_event_cb(cui_Prayers_Button, ui_event_comp_LeftPanel_Prayers_Button, LV_EVENT_ALL, children);
    lv_obj_add_event_cb(cui_Setup_Button, ui_event_comp_LeftPanel_Setup_Button, LV_EVENT_ALL, children);
    lv_obj_add_event_cb(cui_Settings_Button, ui_event_comp_LeftPanel_Settings_Button, LV_EVENT_ALL, children);
    ui_comp_LeftPanel_create_hook(cui_LeftPanel);
    return cui_LeftPanel;
}
