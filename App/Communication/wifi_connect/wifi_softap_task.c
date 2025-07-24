#include "wifi_softap_task.h"
#include "esp_log.h"
#include "wifi_ap_sta.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>

#define TAG "HOTSPOT_TASK"

void hotspot_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Starting WiFi SoftAP...");
    wifi_config_start_softap();   // 初始化 SoftAP 模式

    while (1) {
        // 可添加状态检测、LED 状态输出

        ESP_LOGI("MEM", "Free heap: %" PRIu32, esp_get_free_heap_size());
        // ESP_LOGI("MEM", "Free internal heap: %u", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        // ESP_LOGI("MEM", "Free PSRAM: %u", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));


        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
