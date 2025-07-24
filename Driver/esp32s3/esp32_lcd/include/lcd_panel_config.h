// lcd_panel_config.h
#pragma once

#include "driver/gpio.h"
#include "driver/spi_master.h"

// LCD 分辨率
#define LCD_H_RES              (240)
#define LCD_V_RES              (240)

// LCD 使用的 SPI 控制器
#define LCD_SPI_HOST           (SPI3_HOST)

// LCD SPI 引脚（根据 bsp/sparkbot_board.h 中定义）
#define LCD_PIN_SCK            GPIO_NUM_21
#define LCD_PIN_MOSI           GPIO_NUM_47
#define LCD_PIN_CS             GPIO_NUM_44
#define LCD_PIN_DC             GPIO_NUM_43
#define LCD_PIN_RST            GPIO_NUM_NC   // 如果没有接就设为 GPIO_NUM_NC
#define LCD_PIN_BL             GPIO_NUM_46   // 背光

// LCD 控制参数
#define LCD_PIXEL_CLOCK_HZ     (20 * 1000 * 1000)
#define LCD_COLOR_SPACE        ESP_LCD_COLOR_SPACE_RGB
#define LCD_BITS_PER_PIXEL     16
#define LCD_COLOR_FORMAT        (ESP_LCD_COLOR_FORMAT_RGB565)
#define LCD_CMD_BITS         (8)
#define LCD_PARAM_BITS       (8)
