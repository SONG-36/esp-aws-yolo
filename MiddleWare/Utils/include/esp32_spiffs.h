#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 挂载 SPIFFS 文件系统
 *
 * @return
 *     - ESP_OK: 挂载成功
 *     - 其他: 挂载或获取信息失败
 */
esp_err_t spiffs_mount(void);

/**
 * @brief 卸载 SPIFFS 文件系统
 *
 * @return
 *     - ESP_OK: 卸载成功
 *     - 其他: 卸载失败
 */
esp_err_t spiffs_unmount(void);

#ifdef __cplusplus
}
#endif
