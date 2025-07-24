#include "driver/i2c.h"
#include "i2c_hal.h"
#include "camera_config_table.h"

static bool i2c_initialized = false;

esp_err_t i2c_hal_init(void)
{
    if (i2c_initialized) {
        return ESP_OK;
    }

    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = BSP_I2C_SDA,     
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = BSP_I2C_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000
    };

    ESP_ERROR_CHECK(i2c_param_config(BSP_I2C_NUM, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(BSP_I2C_NUM, i2c_conf.mode, 0, 0, 0));

    i2c_initialized = true;
    return ESP_OK;
}

esp_err_t i2c_hal_deinit(void)
{
    ESP_ERROR_CHECK(i2c_driver_delete(BSP_I2C_NUM));
    i2c_initialized = false;
    return ESP_OK;
}