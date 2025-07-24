#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "task_entry.h"
#include "task_manager.h"
// #include "esp32_spiffs.h"  // SPIFFS 挂载
#include "nvs_flash.h"

extern void register_all_tasks(void);

void app_main(void) {
    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    
    register_all_tasks();               // App 注册任务表给 Core
    task_registry_init();              // Core 遍历任务表
    // spiffs_mount();             // 挂载 SPIFFS

    // esp_err_t err = nvs_flash_init();
    // if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     err = nvs_flash_init();
    // }
    // hal_camera_init();             // 初始化摄像头  

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
    // wifi_config_start_softap();
  

}