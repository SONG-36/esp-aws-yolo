#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化 LCD（SPI + IO + 控制器）
void lcd_init(void);

// 清空屏幕（填充背景色）
void lcd_clear(uint16_t color);

// 显示 RGB565 图像（表情或视频帧）
void lcd_draw_bitmap(int x, int y, int w, int h, const uint16_t *pixels);

// 在屏幕上画边框
void lcd_draw_rect(int x, int y, int w, int h, uint16_t color);

// 控制 LCD 背光
void lcd_backlight_on(void);
void lcd_backlight_off(void);

#ifdef __cplusplus
}
#endif
