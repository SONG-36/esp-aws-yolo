file(GLOB_RECURSE HAL_DRIVER_SRC
    "src/*.c"
)

idf_component_register(
    SRCS ${HAL_DRIVER_SRC}
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp32-camera
)

set(CONFIG_HAL_DRIVER_KCONFIG ${CMAKE_CURRENT_LIST_DIR}/Kconfig)