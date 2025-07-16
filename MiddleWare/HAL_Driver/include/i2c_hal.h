#ifndef _I2C_HAL_H_
#define _I2C_HAL_H_

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t i2c_hal_init(void);
esp_err_t i2c_hal_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
