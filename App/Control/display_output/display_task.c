#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "display_output.h"
#include <inttypes.h>

static const char* TAG = "task_manager";

void display_task(void* arg) {
    display_output_init();

    while (1) {
        ESP_LOGI("MEM", "Free heap: %" PRIu32, esp_get_free_heap_size());
        display_output_show_camera_image();
        vTaskDelay(pdMS_TO_TICKS(100));  // 可调帧率
    }  
}