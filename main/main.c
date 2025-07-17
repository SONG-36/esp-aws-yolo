#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "task_entry.h"
#include "task_manager.h"
#include "camera_hal.h"
// #include "esp_psram.h"
#include "lcd_driver.h"

extern void register_all_tasks(void);

void app_main(void) {
    register_all_tasks();               // App 注册任务表给 Core
    task_registry_init();              // Core 遍历任务表
    // camera_hal_init();             // 初始化摄像头  

    // camera_fb_t *fb = esp_camera_fb_get();
    // if (fb) {
    //     ESP_LOGI("MAIN", " Got image: width=%d, height=%d, len=%d, format=%d",
    //              fb->width, fb->height, fb->len, fb->format);

    //     esp_camera_fb_return(fb);
    // } else {
    //     ESP_LOGE("MAIN", " Failed to capture image");
    // }

    lcd_init();

    // 测试背光
    lcd_backlight_on();

    // 清屏为黑色
    lcd_clear(0x0000);  // 黑色

    // 显示 smile 表情
    // lcd_draw_bitmap(80, 80, 80, 80, img_smile);  

    vTaskDelay(pdMS_TO_TICKS(1000));

    // 绘制红色框
    lcd_draw_rect(60, 60, 120, 120, 0xF800);  // 红色

    // 测试背光关闭
    lcd_backlight_off();
}