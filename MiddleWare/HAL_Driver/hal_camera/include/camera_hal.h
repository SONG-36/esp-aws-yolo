#pragma once

#include "esp_err.h"
#include "esp_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

// 初始化摄像头
esp_err_t hal_camera_init(void);   
// 释放摄像头资源                   
esp_err_t hal_camera_deinit(void);                 
// 捕获一帧图像
camera_fb_t *hal_camera_capture(void);              
// 释放捕获的帧缓存
void hal_camera_release(camera_fb_t *frame); 
// 获取当前帧计数
uint32_t hal_camera_get_frame_counter(void);

#ifdef __cplusplus
}
#endif
