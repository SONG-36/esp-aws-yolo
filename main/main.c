#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "task_entry.h"
#include "task_manager.h"
#include "camera_hal.h"
// #include "esp_psram.h"

extern void register_all_tasks(void);

void app_main(void) {
    register_all_tasks();               // App 注册任务表给 Core
    task_registry_init();              // Core 遍历任务表
    camera_hal_init();             // 初始化摄像头  

    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
        ESP_LOGI("MAIN", " Got image: width=%d, height=%d, len=%d, format=%d",
                 fb->width, fb->height, fb->len, fb->format);

        esp_camera_fb_return(fb);
    } else {
        ESP_LOGE("MAIN", " Failed to capture image");
    }
}