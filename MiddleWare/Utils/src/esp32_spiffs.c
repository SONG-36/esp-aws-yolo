#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_check.h"
#include "esp_vfs_fat.h"
#include "esp32_spiffs.h"

static const char *TAG = "SPIFFS_Manager";

esp_err_t spiffs_mount(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = CONFIG_BSP_SPIFFS_MOUNT_POINT,
        .partition_label = CONFIG_BSP_SPIFFS_PARTITION_LABEL,
        .max_files = CONFIG_BSP_SPIFFS_MAX_FILES,
#ifdef CONFIG_BSP_SPIFFS_FORMAT_ON_MOUNT_FAIL
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif
    };

    esp_err_t ret_val = esp_vfs_spiffs_register(&conf);
    if (ret_val != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SPIFFS (%s)", esp_err_to_name(ret_val));
        return ret_val;
    }

    size_t total = 0, used = 0;
    ret_val = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret_val != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS info (%s)", esp_err_to_name(ret_val));
    } else {
        ESP_LOGI(TAG, "SPIFFS Mounted: Total=%d bytes, Used=%d bytes", total, used);
    }

    return ret_val;
}

esp_err_t spiffs_unmount(void)
{
    esp_err_t ret = esp_vfs_spiffs_unregister(CONFIG_BSP_SPIFFS_PARTITION_LABEL);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS Unmounted");
    } else {
        ESP_LOGE(TAG, "SPIFFS Unmount failed: %s", esp_err_to_name(ret));
    }
    return ret;
}
