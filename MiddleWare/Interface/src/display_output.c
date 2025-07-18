#include "display_output.h"
#include "camera_hal.h"
#include "esp_log.h"
#include "esp_lcd_panel_ops.h"
#include "lcd_panel_config.h"  // 包含 LCD_H_RES、LCD_V_RES、LCD_PIN_* 等定义
#include "lcd_driver.h"

static const char *TAG = "display_output";

extern esp_lcd_panel_handle_t panel_handle;

void display_output_init(void)
{
    esp_err_t ret;

    // 1. 初始化 LCD
    lcd_init();

    if (!panel_handle) {
        ESP_LOGE(TAG, "LCD panel init failed (panel_handle is NULL)");
        return;
    }

    // 2. 初始化 Camera
    ret = camera_hal_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Display output module initialized.");

    // 3. 清屏或显示初始画面
    static uint16_t clear_color[LCD_H_RES * LCD_V_RES] = {0};  // 黑屏
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, clear_color);
}

void display_output_show_camera_image(void)
{
    camera_fb_t *fb = camera_hal_capture();
    if (!fb) {
        ESP_LOGE(TAG, "Failed to capture frame");
        return;
    }

    if (fb->format != PIXFORMAT_RGB565) {
        ESP_LOGW(TAG, "Unsupported frame format: %d", fb->format);
        camera_hal_release(fb);
        return;
    }

    // 安全检查尺寸是否匹配（可选）
    if (fb->width != LCD_H_RES || fb->height != LCD_V_RES) {
        ESP_LOGW(TAG, "Frame size mismatch: %dx%d", fb->width, fb->height);
    }

    esp_err_t ret = esp_lcd_panel_draw_bitmap(
        panel_handle,
        0, 0,
        fb->width, fb->height,
        fb->buf
    );

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to draw frame to LCD: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGD(TAG, "Frame displayed: %dx%d", fb->width, fb->height);
    }

    camera_hal_release(fb);
}
