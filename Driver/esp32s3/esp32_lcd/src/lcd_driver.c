#include "lcd_driver.h"
#include "lcd_panel_config.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "lcd_driver";

// 面板和 IO 句柄
static esp_lcd_panel_handle_t panel_handle = NULL;
static esp_lcd_panel_io_handle_t io_handle = NULL;

void lcd_init(void)
{
    ESP_LOGI(TAG, "Step 1: Initializing SPI bus...");
    spi_bus_config_t bus_config = {
        .sclk_io_num = LCD_PIN_SCK,
        .mosi_io_num = LCD_PIN_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * LCD_V_RES * 2,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &bus_config, SPI_DMA_CH_AUTO));
    ESP_LOGI(TAG, "SPI bus initialized.");

    ESP_LOGI(TAG, "Step 2: Configuring SPI panel IO...");
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_PIN_DC,
        .cs_gpio_num = LCD_PIN_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(LCD_SPI_HOST, &io_config, &io_handle));
    ESP_LOGI(TAG, "Panel IO configured.");

    ESP_LOGI(TAG, "Step 3: Creating ST7789 panel...");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_RST,
        .color_space = LCD_COLOR_SPACE,
        .bits_per_pixel = LCD_BITS_PER_PIXEL,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
    ESP_LOGI(TAG, "ST7789 panel created.");

    ESP_LOGI(TAG, "Step 4: Initializing panel...");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    ESP_LOGI(TAG, "Panel initialized and configured.");

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
#else
    ESP_ERROR_CHECK(esp_lcd_panel_disp_off(panel_handle, false));
#endif
    ESP_LOGI(TAG, "Display turned on.");

    ESP_LOGI(TAG, "Step 5: Configuring backlight GPIO...");
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PIN_BL,
        .pull_down_en = 0,
        .pull_up_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gpio_set_level(LCD_PIN_BL, 1);
    ESP_LOGI(TAG, "Backlight enabled.");

    ESP_LOGI(TAG, "LCD initialization complete.");
}

void lcd_clear(uint16_t color)
{
    ESP_LOGI(TAG, "Clearing LCD with color 0x%04X...", color);
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, (const void *)(&color));
    ESP_LOGI(TAG, "LCD cleared.");
}

void lcd_draw_bitmap(int x, int y, int w, int h, const uint16_t *pixels)
{
    ESP_LOGI(TAG, "Drawing bitmap at (%d,%d) size %dx%d", x, y, w, h);
    esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + w, y + h, (const void *)pixels);
}

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

void lcd_backlight_on(void)
{
    ESP_LOGI(TAG, "Backlight ON.");
    gpio_set_level(LCD_PIN_BL, 1);
}

void lcd_backlight_off(void)
{
    ESP_LOGI(TAG, "Backlight OFF.");
    gpio_set_level(LCD_PIN_BL, 0);
}
