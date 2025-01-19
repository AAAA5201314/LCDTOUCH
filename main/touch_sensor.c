#include <stdio.h>
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "touch_sensor.h"

// I2C 初始化函数
esp_err_t i2c_master_init() {
    static i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

// 复位 FT6336G
void reset_ft6336g() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << RSTN_GPIO);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // 拉低 RSTN 引脚
    gpio_set_level(RSTN_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(RSTN_DELAY_MS)); // 保持低电平至少 5ms

    // 释放 RSTN 引脚
    gpio_set_level(RSTN_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(RSTN_DELAY_MS)); // 等待复位完成
}

// 初始化 INT 引脚
void init_int_pin() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << INT_GPIO);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

// 从设备读取寄存器的值
esp_err_t read_register_value(uint8_t device_address, uint8_t register_address, uint8_t *value) {
    esp_err_t err;
    uint8_t write_buffer[1] = { register_address }; // 要写入的寄存器地址
    uint8_t read_buffer[1]; // 用于存储读取的值

    // 发送寄存器地址
    err = i2c_master_write_to_device(I2C_MASTER_NUM, device_address, write_buffer, sizeof(write_buffer), I2C_MASTER_TIMEOUT_MS);
    if (err != ESP_OK) {
        return err;
    }

    // 读取寄存器值
    err = i2c_master_read_from_device(I2C_MASTER_NUM, device_address, read_buffer, sizeof(read_buffer), I2C_MASTER_TIMEOUT_MS);
    if (err != ESP_OK) {
        return err;
    }

    *value = read_buffer[0]; // 将读取的值存储到传入的指针中
    return ESP_OK;
}

// 获取触摸点数
esp_err_t get_touch_count(uint8_t *touch_count) {
    return read_register_value(DEVICE_ADDRESS, TD_STATUS_REG, touch_count);
}

// 获取触摸坐标
esp_err_t get_touch_coordinates(uint8_t index, uint16_t *x, uint16_t *y) {
    uint8_t p_xh, p_xl, p_yh, p_yl;
    esp_err_t ret;

    if (index == 0) {
        ret = read_register_value(DEVICE_ADDRESS, P1_XH_REG, &p_xh);
        if (ret != ESP_OK) return ret;

        ret = read_register_value(DEVICE_ADDRESS, P1_XL_REG, &p_xl);
        if (ret != ESP_OK) return ret;

        ret = read_register_value(DEVICE_ADDRESS, P1_YH_REG, &p_yh);
        if (ret != ESP_OK) return ret;

        ret = read_register_value(DEVICE_ADDRESS, P1_YL_REG, &p_yl);
        if (ret != ESP_OK) return ret;
    } else if (index == 1) {
        ret = read_register_value(DEVICE_ADDRESS, P2_XH_REG, &p_xh);
        if (ret != ESP_OK) return ret;

        ret = read_register_value(DEVICE_ADDRESS, P2_XL_REG, &p_xl);
        if (ret != ESP_OK) return ret;

        ret = read_register_value(DEVICE_ADDRESS, P2_YH_REG, &p_yh);
        if (ret != ESP_OK) return ret;

        ret = read_register_value(DEVICE_ADDRESS, P2_YL_REG, &p_yl);
        if (ret != ESP_OK) return ret;
    } else {
        return ESP_ERR_INVALID_ARG; // 目前只支持两个触摸点
    }

    // 计算完整的坐标
    *x = ((p_xh & 0x0F) << 8) | p_xl; // 高4位和低8位组合
    *y = ((p_yh & 0x0F) << 8) | p_yl; // 高4位和低8位组合

    return ESP_OK;
}

// 总的初始化方法
esp_err_t touch_sensor_init() {
    esp_err_t ret;

    // 初始化 I2C
    ret = i2c_master_init();
    if (ret != ESP_OK) {
        return ret;
    }

    // 复位 FT6336G
    reset_ft6336g();

    // 初始化 INT 引脚
    init_int_pin();

    return ESP_OK;
} 