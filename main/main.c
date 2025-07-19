#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "task_entry.h"
#include "task_manager.h"
#include "camera_hal.h"
// #include "esp_psram.h"
#include "lcd_driver.h"
#include "display_output.h"
#include "esp32_spiffs.h"  // SPIFFS 挂载

extern void register_all_tasks(void);

void app_main(void) {
    register_all_tasks();               // App 注册任务表给 Core
    task_registry_init();              // Core 遍历任务表
    spiffs_mount();             // 挂载 SPIFFS
    // camera_hal_init();             // 初始化摄像头  

    // camera_fb_t *fb = esp_camera_fb_get();
    // if (fb) {
    //     ESP_LOGI("MAIN", " Got image: width=%d, height=%d, len=%d, format=%d",
    //              fb->width, fb->height, fb->len, fb->format);

    //     esp_camera_fb_return(fb);
    // } else {
    //     ESP_LOGE("MAIN", " Failed to capture image");
    // }

    // lcd_init();

    // // 准备颜色数组（RGB565）

    // const uint16_t test_colors[] = {
    //     0xF800, // Red
    //     0x07E0, // Green
    //     0x001F, // Blue
    //     0xFFFF, // White
    //     0x0000, // Black
    // };

    // while (1) {
    //     for (int i = 0; i < sizeof(test_colors)/sizeof(test_colors[0]); i++) {
    //         ESP_LOGI("MAIN", "Color: 0x%04X", test_colors[i]);
    //         lcd_clear(test_colors[i]);
    //         vTaskDelay(pdMS_TO_TICKS(1000));
    //     }
    // }

    display_output_init();

    while (1) {
        display_output_show_camera_image();
        vTaskDelay(pdMS_TO_TICKS(100));  // 可调帧率
    }    

}