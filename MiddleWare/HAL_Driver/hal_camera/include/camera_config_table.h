#pragma once

#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_camera.h"
#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/* I2C */
#define BSP_I2C_SCL           (GPIO_NUM_5)
#define BSP_I2C_SDA           (GPIO_NUM_4)

/**********************************************
 * Camera GPIO Pins
 **********************************************/
#define BSP_CAMERA_XCLK      (GPIO_NUM_15)
#define BSP_CAMERA_PCLK      (GPIO_NUM_13)
#define BSP_CAMERA_VSYNC     (GPIO_NUM_6)
#define BSP_CAMERA_HSYNC     (GPIO_NUM_7)
#define BSP_CAMERA_D0        (GPIO_NUM_11)
#define BSP_CAMERA_D1        (GPIO_NUM_9)
#define BSP_CAMERA_D2        (GPIO_NUM_8)
#define BSP_CAMERA_D3        (GPIO_NUM_10)
#define BSP_CAMERA_D4        (GPIO_NUM_12)
#define BSP_CAMERA_D5        (GPIO_NUM_18)
#define BSP_CAMERA_D6        (GPIO_NUM_17)
#define BSP_CAMERA_D7        (GPIO_NUM_16)

/**********************************************
 * I2C Port for SCCB (camera control)
 **********************************************/
#define BSP_I2C_NUM CONFIG_BSP_I2C_NUM

/**********************************************
 * Camera Default Configuration
 **********************************************/
#define BSP_CAMERA_DEFAULT_CONFIG         \
    {                                     \
        .pin_pwdn = GPIO_NUM_NC,          \
        .pin_reset = GPIO_NUM_NC,         \
        .pin_xclk = BSP_CAMERA_XCLK,      \
        .pin_sccb_sda = GPIO_NUM_NC,      \
        .pin_sccb_scl = GPIO_NUM_NC,      \
        .pin_d7 = BSP_CAMERA_D7,          \
        .pin_d6 = BSP_CAMERA_D6,          \
        .pin_d5 = BSP_CAMERA_D5,          \
        .pin_d4 = BSP_CAMERA_D4,          \
        .pin_d3 = BSP_CAMERA_D3,          \
        .pin_d2 = BSP_CAMERA_D2,          \
        .pin_d1 = BSP_CAMERA_D1,          \
        .pin_d0 = BSP_CAMERA_D0,          \
        .pin_vsync = BSP_CAMERA_VSYNC,    \
        .pin_href = BSP_CAMERA_HSYNC,     \
        .pin_pclk = BSP_CAMERA_PCLK,      \
        .xclk_freq_hz = 16000000,         \
        .ledc_timer = LEDC_TIMER_0,       \
        .ledc_channel = LEDC_CHANNEL_0,   \
        .pixel_format = PIXFORMAT_RGB565, \
        .frame_size = FRAMESIZE_240X240,  \
        .jpeg_quality = 12,               \
        .fb_count = 2,                    \
        .fb_location = CAMERA_FB_IN_PSRAM,\
        .sccb_i2c_port = BSP_I2C_NUM,     \
    }

/**********************************************
 * Optional VFlip / HMirror
 **********************************************/
#define BSP_CAMERA_VFLIP   1
#define BSP_CAMERA_HMIRROR 0

#ifdef __cplusplus
}
#endif
