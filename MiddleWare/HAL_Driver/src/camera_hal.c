#include "camera_hal.h"
#include "camera_config_table.h"
#include "esp_log.h"
#include "i2c_hal.h"
// #include "esp_psram.h"

static const char* TAG = "camera_module";

esp_err_t camera_hal_init(void)
{
    // 1 初始化 I2C
    ESP_ERROR_CHECK(i2c_hal_init());  // 初始化 I2C

    // 2 准备配置结构体
    camera_config_t config = BSP_CAMERA_DEFAULT_CONFIG;

    // 3 Log关键字段，确保GPIO没错
    ESP_LOGI(TAG, "Camera XCLK: %d, SCCB SDA: %d, SCCB SCL: %d", 
             config.pin_xclk, config.pin_sccb_sda, config.pin_sccb_scl);
    ESP_LOGI(TAG, "Camera Frame Size: %d, Pixel Format: %d", 
             config.frame_size, config.pixel_format);

    // 4 初始化摄像头
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE("CAM", "Camera init failed! err=0x%x", err);
        return err;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s) {
        s->set_vflip(s, BSP_CAMERA_VFLIP);
        s->set_hmirror(s, BSP_CAMERA_HMIRROR);
    }

    ESP_LOGI("CAM", "Camera init success");
    return ESP_OK;
}

esp_err_t camera_hal_deinit(void)
{
    return esp_camera_deinit();
}

camera_fb_t* camera_hal_capture(void) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        printf("camera_hal_capture: Failed to get frame\n");
        return NULL;
    }

    return fb;
}
