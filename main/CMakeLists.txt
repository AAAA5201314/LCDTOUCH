# main/CMakeLists.txt

# 注册组件
idf_component_register(
    SRCS "spi_lcd_touch_example_main.c" "lvgl_demo_ui.c" "touch_sensor.c"  # 添加所有源文件
    INCLUDE_DIRS "."  # 添加头文件目录
)