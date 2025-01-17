#ifndef TOUCH_SENSOR_H
#define TOUCH_SENSOR_H

#include "esp_err.h"

// I2C 配置
#define I2C_MASTER_SCL_IO 39               // SCL GPIO
#define I2C_MASTER_SDA_IO 40               // SDA GPIO
#define I2C_MASTER_NUM I2C_NUM_0           // I2C 端口号
#define I2C_MASTER_FREQ_HZ 100000          // I2C 频率
#define I2C_MASTER_TIMEOUT_MS 1000         // 超时时间

// 设备地址
#define DEVICE_ADDRESS 0x38                // I2C 设备地址

// 寄存器地址
#define TD_STATUS_REG 0x02                 // 报点个数寄存器
#define P1_XH_REG 0x03                     // 第1点的X坐标高4位寄存器
#define P1_XL_REG 0x04                     // 第1点的X坐标低8位寄存器
#define P1_YH_REG 0x05                     // 第1点的Y坐标高4位寄存器
#define P1_YL_REG 0x06                     // 第1点的Y坐标低8位寄存器
#define P2_XH_REG 0x09                     // 第2点的X坐标高4位寄存器
#define P2_XL_REG 0x0A                     // 第2点的X坐标低8位寄存器
#define P2_YH_REG 0x0B                     // 第2点的Y坐标高4位寄存器
#define P2_YL_REG 0x0C                     // 第2点的Y坐标低8位寄存器

// RSTN 引脚配置
#define RSTN_GPIO 41                        // RSTN 引脚 GPIO
#define RSTN_DELAY_MS 10                   // 复位信号保持时间（至少 5ms）

// INT 引脚配置
#define INT_GPIO 16                        // INT 引脚 GPIO

// 函数声明
esp_err_t i2c_master_init();
void reset_ft6336g();
void init_int_pin();
esp_err_t read_register_value(uint8_t device_address, uint8_t register_address, uint8_t *value);
esp_err_t get_touch_count(uint8_t *touch_count);
esp_err_t get_touch_coordinates(uint8_t index, uint16_t *x, uint16_t *y);
esp_err_t touch_sensor_init();

#endif // TOUCH_SENSOR_H 