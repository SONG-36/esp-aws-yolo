file(GLOB_RECURSE INTERFACE
    "src/*.c"
)

idf_component_register(
    SRCS ${INTERFACE}
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp32-camera
        esp32_lcd
        HAL_Driver
)