#ifndef WIFI_WINDOW_H
#define WIFI_WINDOW_H

#include <string>
#include <vector>
#include <lvgl.h> // Assuming lvgl.h is the header for the LVGL library

class WifiScreenModel {
public:
    WifiScreenModel();
    void set_wifi_connected(bool connected);
    bool is_wifi_connected() const;
    void set_ssid(const std::string& new_ssid);
    std::string get_ssid() const;
    void set_password(const std::string& new_password);
    std::string get_password() const;
    void set_wifi_list(const std::vector<std::string>& new_wifi_list);
    const std::vector<std::string>& get_wifi_list() const;

private:
    bool wifi_connected;
    std::string ssid;
    std::string password;
    std::vector<std::string> wifi_list;
};

class WifiScreenView {
public:
    WifiScreenView(lv_obj_t* active_screen) {
        setup_scr_screen(active_screen);
    }

    void setup_scr_screen(lv_obj_t* active_screen);
    void set_wifi_list(const std::vector<std::string>& wifi_list);
    void navigate_to_con_tab(const std::string& ssid);

private:
    lv_obj_t* tabview;
    lv_obj_t* label_rssi;
    lv_obj_t* bar_rssi;
    lv_obj_t* ta_ssid;
    lv_obj_t* ta_password;
    lv_obj_t* list;
    lv_obj_t* btn_disconnect;
    lv_obj_t* btn_connect;

    void init_style();
    void init_style_disabled(lv_style_t* style_disabled);
    void create_wifi_tab(lv_obj_t* tabview);
    void create_con_tab(lv_obj_t* tabview);
    void create_list_tab(lv_obj_t* tabview);

    static void btn_disconnect_event_handler(lv_event_t* e);
    static void btn_connect_event_handler(lv_event_t* e);
    static void btn_wifi_item_event_handler(lv_event_t* e);
};
class WifiScreenController {
public:
    WifiScreenController(WifiScreenModel& model, WifiScreenView& view);
    void bind_wifi_list_to_view();

private:
    WifiScreenModel& model;
    WifiScreenView& view;
};

#endif // WIFI_WINDOW_H
