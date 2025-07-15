#pragma once

#include "esp_err.h"
#include "esp_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t camera_hal_init(void);

esp_err_t camera_hal_deinit(void);

camera_fb_t *camera_hal_capture(void);

#ifdef __cplusplus
}
#endif
