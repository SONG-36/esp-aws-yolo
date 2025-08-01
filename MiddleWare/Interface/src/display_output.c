#include "display_output.h"
#include "camera_hal.h"
#include "esp_log.h"
#include "esp_lcd_panel_ops.h"
#include "lcd_panel_config.h"  // 包含 LCD_H_RES、LCD_V_RES、LCD_PIN_* 等定义
#include "lcd_driver.h"
#include "driver/uart.h"
#include "string.h"
#include "freertos/portmacro.h"   // 定义 portMUX_TYPE 和宏
#include "freertos/semphr.h"      // 可选，用于其他同步机制

static const char *TAG = "display_output";
portMUX_TYPE spi_mux = portMUX_INITIALIZER_UNLOCKED;

extern esp_lcd_panel_handle_t panel_handle;

static void flip_image_y(uint16_t *dst, const uint16_t *src, int width, int height)
{
    for (int y = 0; y < height; y++) {
        const uint16_t *src_row = src + (height - 1 - y) * width;
        uint16_t *dst_row = dst + y * width;
        memcpy(dst_row, src_row, width * sizeof(uint16_t));
    }
}

void display_output_init(void)
{
    esp_err_t ret;

    // 1. 初始化 LCD
    lcd_init();

    if (!panel_handle) {
        ESP_LOGE(TAG, "LCD panel init failed (panel_handle is NULL)");
        return;
    }

    //2. 初始化 Camera
    ret = hal_camera_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Display output module initialized.");

    // 3. 清屏或显示初始画面
    static uint16_t clear_color[LCD_H_RES * LCD_V_RES] = {0};  // 黑屏
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, clear_color);
}


// void display_output_show_camera_image(void)
// {
//     static bool once = false;
//     static uint16_t *red_image = NULL;

//     if (!once) {
//         red_image = heap_caps_malloc(240 * 240 * sizeof(uint16_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//         if (!red_image) {
//             ESP_LOGE(TAG, "Red image malloc failed");
//             return;
//         }
//         for (int i = 0; i < 240 * 240; i++) red_image[i] = 0xF800;
//         once = true;
//     }

//     lcd_draw_bitmap(0, 0, 240, 240, red_image);
// }


void display_output_show_camera_image(void)
{
    camera_fb_t *fb = hal_camera_capture();
    if (!fb) {
        ESP_LOGE(TAG, "Failed to capture image");
        return;
    }

    if (fb->format == PIXFORMAT_RGB565) {
        // 分配用于翻转显示的临时 buffer
        uint16_t *flipped_buf = heap_caps_malloc(fb->len, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

        if (!flipped_buf) {
            ESP_LOGE(TAG, "Failed to allocate buffer for flip");
            hal_camera_release(fb);
            return;
        }

        flip_image_y(flipped_buf, (uint16_t *)fb->buf, fb->width, fb->height);

        // taskENTER_CRITICAL(&spi_mux);
        lcd_draw_bitmap(0, 0, fb->width, fb->height, flipped_buf);
        // taskEXIT_CRITICAL(&spi_mux);

        free(flipped_buf);
    } else if (fb->format == PIXFORMAT_JPEG) {
        ESP_LOGW(TAG, "JPEG format received — skipping display");
    }

    hal_camera_release(fb);
}

