#include "lcd_driver.h"
#include "lcd_panel_config.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "lcd_driver";

// LCD 面板和 IO 接口句柄（全局）
esp_lcd_panel_handle_t panel_handle = NULL;
esp_lcd_panel_io_handle_t io_handle = NULL;

/**
 * @brief 创建 LCD SPI 接口及面板实例
 *        包括 SPI 总线配置、IO 接口创建、面板创建
 */
void lcd_create_panel(void)
{
    ESP_LOGI(TAG, "Creating SPI panel interface...");

    // SPI 总线配置
    spi_bus_config_t bus_config = {
        .sclk_io_num = LCD_PIN_SCK,
        .mosi_io_num = LCD_PIN_MOSI,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        // .max_transfer_sz = LCD_H_RES * LCD_V_RES * 2,
        .max_transfer_sz = 2 * 1024,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &bus_config, SPI_DMA_CH_AUTO));

    // SPI IO 接口配置
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_PIN_DC,
        .cs_gpio_num = LCD_PIN_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .flags = {
            .dc_low_on_data = 0,
            .dc_low_on_param = 0,
        },
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(LCD_SPI_HOST, &io_config, &io_handle));

     // 创建面板实例（ST7789）
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_RST,
        .color_space = LCD_COLOR_SPACE,
        .bits_per_pixel = LCD_BITS_PER_PIXEL,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    ESP_LOGI(TAG, "SPI panel interface created.");
}

/**
 * @brief 初始化 LCD 面板并打开背光
 */
void lcd_start_panel(void)
{
    ESP_LOGI(TAG, "Reset/init panel...");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    // 背光 GPIO 配置
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PIN_BL,
        .pull_down_en = 0,
        .pull_up_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gpio_set_level(LCD_PIN_BL, 1);
    ESP_LOGI(TAG, "Backlight enabled");
}

/**
 * @brief 一键初始化 LCD，包括面板和背光
 */
void lcd_init(void)
{
    lcd_create_panel();
    lcd_start_panel();
}

/**
 * @brief 用指定颜色清空整个屏幕
 * @param color 16bit RGB565 颜色值
 */
void lcd_clear(uint16_t color)
{
    size_t buffer_size = LCD_H_RES * LCD_V_RES * sizeof(uint16_t);
    uint16_t *buf = heap_caps_malloc(buffer_size, MALLOC_CAP_DMA);
    if (!buf) {
        ESP_LOGE(TAG, "Failed to alloc clear buffer");
        return;
    }

    for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
        buf[i] = color;
    }

    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, buf);
    free(buf);
}

/**
 * @brief 绘制图像到屏幕指定区域
 * @param x 起始 x 坐标
 * @param y 起始 y 坐标
 * @param w 图像宽度
 * @param h 图像高度
 * @param pixels 图像像素数据（RGB565）
 */
void lcd_draw_bitmap(int x, int y, int w, int h, const uint16_t *pixels)
{
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + w, y + h, (const void *)pixels);
}

/**
 * @brief 绘制矩形边框（非填充）
 */
void lcd_draw_rect(int x, int y, int w, int h, uint16_t color)
{
    ESP_LOGI(TAG, "Drawing rectangle at (%d,%d), size %dx%d, color 0x%04X", x, y, w, h, color);
    static uint16_t line_buf[240];
    for (int i = 0; i < w; i++) line_buf[i] = color;

    lcd_draw_bitmap(x, y, w, 1, line_buf);             // top
    lcd_draw_bitmap(x, y + h - 1, w, 1, line_buf);     // bottom

    for (int i = 0; i < h; i++) lcd_draw_bitmap(x, y + i, 1, 1, &color);           // left
    for (int i = 0; i < h; i++) lcd_draw_bitmap(x + w - 1, y + i, 1, 1, &color);   // right
    ESP_LOGI(TAG, "Rectangle drawn.");
}

/**
 * @brief 打开背光
 */
void lcd_backlight_on(void)
{
    ESP_LOGI(TAG, "Backlight ON.");
    gpio_set_level(LCD_PIN_BL, 1);
}

/**
 * @brief 关闭背光
 */
void lcd_backlight_off(void)
{
    ESP_LOGI(TAG, "Backlight OFF.");
    gpio_set_level(LCD_PIN_BL, 0);
}
