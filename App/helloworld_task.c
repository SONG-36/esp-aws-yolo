#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG = "task_manager";

void hello_task(void* arg) {
    while (1) {
        ESP_LOGI(TAG, "Hello World!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}