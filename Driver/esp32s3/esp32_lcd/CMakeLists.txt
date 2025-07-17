file(GLOB_RECURSE LCD_DRIVER_SRC
    "src/*.c"
)

idf_component_register(
    SRCS ${LCD_DRIVER_SRC}
    INCLUDE_DIRS
        "include"

    REQUIRES esp_lcd driver
)