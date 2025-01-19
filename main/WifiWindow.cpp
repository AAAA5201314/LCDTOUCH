#include "WifiWindow.h"

WifiScreenModel::WifiScreenModel() : wifi_connected(false), ssid(""), password("") {}

void WifiScreenModel::set_wifi_connected(bool connected) {
    wifi_connected = connected;
}

bool WifiScreenModel::is_wifi_connected() const {
    return wifi_connected;
}

void WifiScreenModel::set_ssid(const std::string& new_ssid) {
    ssid = new_ssid;
}

std::string WifiScreenModel::get_ssid() const {
    return ssid;
}

void WifiScreenModel::set_password(const std::string& new_password) {
    password = new_password;
}

std::string WifiScreenModel::get_password() const {
    return password;
}

void WifiScreenModel::set_wifi_list(const std::vector<std::string>& new_wifi_list) {
    wifi_list = new_wifi_list;
}

const std::vector<std::string>& WifiScreenModel::get_wifi_list() const {
    return wifi_list;
}

void WifiScreenView::setup_scr_screen(lv_obj_t* active_screen) {
    tabview = lv_obj_create(NULL);
    lv_obj_clear_flag(tabview, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    tabview = lv_tabview_create(active_screen, LV_DIR_TOP, 50);
    lv_obj_set_width(tabview, 320);
    lv_obj_set_height(tabview, 480);
    lv_obj_set_align(tabview, LV_ALIGN_CENTER);
    lv_obj_clear_flag(tabview, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    create_wifi_tab(tabview);
    create_con_tab(tabview);
    create_list_tab(tabview);

    init_style();
}

void WifiScreenView::init_style() {
    lv_obj_set_style_bg_color(tabview, lv_color_hex(0xeaeff3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(tabview, lv_color_hex(0xcccccc), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tabview, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(tabview, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void WifiScreenView::create_wifi_tab(lv_obj_t* tabview) {
    lv_obj_t* wifi_tab = lv_tabview_add_tab(tabview, "WIFI");
    label_rssi = lv_label_create(wifi_tab); // Assign to global variable
    lv_label_set_text(label_rssi, "WIFI RSSI");
    lv_obj_set_x(label_rssi, 10);
    lv_obj_set_y(label_rssi, 10);

    bar_rssi = lv_bar_create(wifi_tab); // Assign to global variable
    lv_bar_set_range(bar_rssi, 0, 100);
    lv_bar_set_value(bar_rssi, 50, LV_ANIM_OFF);
    lv_obj_set_width(bar_rssi, 75);
    lv_obj_set_height(bar_rssi, 286);
    lv_obj_set_x(bar_rssi, -92);
    lv_obj_set_y(bar_rssi, -11);
    lv_obj_set_align(bar_rssi, LV_ALIGN_CENTER);

    //Compensating for LVGL9.1 draw crash with bar/slider max value when top-padding is nonzero and right-padding is 0
    if(lv_obj_get_style_pad_top(bar_rssi, LV_PART_MAIN) > 0) lv_obj_set_style_pad_right(bar_rssi,
                                                                                              lv_obj_get_style_pad_right(bar_rssi, LV_PART_MAIN) + 1, LV_PART_MAIN);
    btn_disconnect = lv_btn_create(wifi_tab); // Assign to global variable
    lv_obj_set_width(btn_disconnect, 100);
    lv_obj_set_height(btn_disconnect, 104);
    lv_obj_set_x(btn_disconnect, 66);
    lv_obj_set_y(btn_disconnect, 1);
    lv_obj_set_align(btn_disconnect, LV_ALIGN_CENTER);
    lv_obj_add_flag(btn_disconnect, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(btn_disconnect, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(btn_disconnect, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* btn_disconnect_label = lv_label_create(btn_disconnect);
    lv_obj_set_width(btn_disconnect_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(btn_disconnect_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(btn_disconnect_label, 1);
    lv_obj_set_y(btn_disconnect_label, 2);
    lv_obj_set_align(btn_disconnect_label, LV_ALIGN_CENTER);
    lv_label_set_text(btn_disconnect_label, "DisCon");

    lv_obj_t* label_rssi_value = lv_label_create(wifi_tab);
    lv_label_set_text(label_rssi_value, "-50");
    lv_obj_set_x(label_rssi_value, 60);
    lv_obj_set_y(label_rssi_value, 310); // Adjusted position for 90-degree rotation

    // Bind the disconnect button callback
    lv_obj_add_event_cb(btn_disconnect, btn_disconnect_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_add_event_cb(btn_connect, btn_connect_event_handler, LV_EVENT_CLICKED, this);
}

void WifiScreenView::create_con_tab(lv_obj_t* tabview) {
    lv_obj_t* con_tab = lv_tabview_add_tab(tabview, "CON");
    ta_ssid = lv_textarea_create(con_tab); // Assign to global variable
    lv_textarea_set_text(ta_ssid, "13058299352");
    lv_obj_set_width(ta_ssid, 240);
    lv_obj_set_height(ta_ssid, 30);
    lv_obj_set_x(ta_ssid, 20);
    lv_obj_set_y(ta_ssid, -180);
    lv_obj_set_align(ta_ssid, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ta_ssid, "Placeholder...");

    ta_password = lv_textarea_create(con_tab); // Assign to global variable
    lv_textarea_set_text(ta_password, "Hello World");
    lv_obj_set_width(ta_password, 240);
    lv_obj_set_height(ta_password, 30);
    lv_obj_set_x(ta_password, 20);
    lv_obj_set_y(ta_password, -140);
    lv_obj_set_align(ta_password, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ta_password, "Placeholder...");

    lv_obj_t* label_ssid = lv_label_create(con_tab);
    lv_obj_set_width(label_ssid, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(label_ssid, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(label_ssid, -129);
    lv_obj_set_y(label_ssid, -179);
    lv_obj_set_align(label_ssid, LV_ALIGN_CENTER);
    lv_label_set_text(label_ssid, "SSID");

    lv_obj_t* label_password = lv_label_create(con_tab);
    lv_obj_set_width(label_password, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(label_password, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(label_password, -130);
    lv_obj_set_y(label_password, -139);
    lv_obj_set_align(label_password, LV_ALIGN_CENTER);
    lv_label_set_text(label_password, "PASS");

    btn_connect = lv_btn_create(con_tab); // Assign to global variable
    lv_obj_set_width(btn_connect, 100);
    lv_obj_set_height(btn_connect, 50);
    lv_obj_set_x(btn_connect, -11);
    lv_obj_set_y(btn_connect, -84);
    lv_obj_set_align(btn_connect, LV_ALIGN_CENTER);
    lv_obj_add_flag(btn_connect, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(btn_connect, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_t* btn_connect_label = lv_label_create(btn_connect);
    lv_obj_set_width(btn_connect_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(btn_connect_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(btn_connect_label, LV_ALIGN_CENTER);
    lv_label_set_text(btn_connect_label, "CON");

    lv_obj_t* ui_Keyboard1 = lv_keyboard_create(con_tab);
    lv_obj_set_width(ui_Keyboard1, 320);
    lv_obj_set_height(ui_Keyboard1, 120);
    lv_obj_set_x(ui_Keyboard1, 0);
    lv_obj_set_y(ui_Keyboard1, 150);
    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);
}

void WifiScreenView::navigate_to_con_tab(const std::string& ssid) {
    // 切换到“CON”标签
    lv_tabview_set_act(tabview, 1, LV_ANIM_OFF); // 假设“CON”标签是第一个索引

    // 设置SSID文本区域的文本
    lv_textarea_set_text(ta_ssid, ssid.c_str());
}

void WifiScreenView::create_list_tab(lv_obj_t* tabview) {
    lv_obj_t* list_tab = lv_tabview_add_tab(tabview, "LIST");
    list = lv_list_create(list_tab); // Assign to global variable
    lv_obj_set_size(list, 300, 399); // Adjusted size for 90-degree rotation
}

// Callback functions
void WifiScreenView::btn_disconnect_event_handler(lv_event_t* e) {
    // Handle disconnect button click
    WifiScreenView* view = static_cast<WifiScreenView*>(lv_event_get_user_data(e));
    lv_obj_add_state(view->btn_disconnect, LV_STATE_DISABLED);
    lv_obj_clear_state(view->btn_connect, LV_STATE_DISABLED);
}

void WifiScreenView::btn_connect_event_handler(lv_event_t* e) {
    // Handle connect button click
    WifiScreenView* view = static_cast<WifiScreenView*>(lv_event_get_user_data(e));    
    lv_obj_clear_state(view->btn_disconnect, LV_STATE_DISABLED);
    lv_obj_add_state(view->btn_connect, LV_STATE_DISABLED);
}

void WifiScreenView::set_wifi_list(const std::vector<std::string>& wifi_list) {
    // 清空现有的列表项
    while (lv_obj_get_child(list, 0) != NULL) {
        lv_obj_del(lv_obj_get_child(list, 0)); // 删除每个子项（列表项）
    }

    // 添加新的 Wi-Fi 列表项并绑定事件
    for (const auto& ssid : wifi_list) {
        lv_obj_t* list_item = lv_list_add_btn(list, LV_SYMBOL_WIFI, ssid.c_str()); // 添加每个 SSID 到列表

        // 创建指针数组  局部变量会导致一退出就被释放
         void** user_data = new void* [2];
        user_data[0] = this; // 存储 this 指针
        user_data[1] = new std::string(ssid); // 存储 SSID 的指针

        // 绑定事件并传递用户数据
        lv_obj_add_event_cb(list_item, btn_wifi_item_event_handler, LV_EVENT_CLICKED, user_data);
    }
}

// 事件处理函数
void WifiScreenView::btn_wifi_item_event_handler(lv_event_t* e) {
    void** user_data = static_cast<void**>(lv_event_get_user_data(e)); // 获取用户数据
    WifiScreenView* view = static_cast<WifiScreenView*>(user_data[0]); // 获取 this 指针
    std::string* ssid_ptr = static_cast<std::string*>(user_data[1]); // 获取 SSID 指针

    // 使用 view 和 ssid
    view->navigate_to_con_tab(*ssid_ptr); // 传递所选的 SSID

  
}

WifiScreenController::WifiScreenController(WifiScreenModel& model, WifiScreenView& view)
    : model(model), view(view) {}

void WifiScreenController::bind_wifi_list_to_view() {
    const std::vector<std::string>& wifi_list = model.get_wifi_list(); // Get the Wi-Fi list from the model
    // Assuming the view has a method to set the Wi-Fi list
    view.set_wifi_list(wifi_list); // Bind the list to the view
}
