# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

list(APPEND EXTRA_COMPONENT_DIRS
    ${CMAKE_SOURCE_DIR}/App
    ${CMAKE_SOURCE_DIR}/Driver
    ${CMAKE_SOURCE_DIR}/MiddleWare/Core
    ${CMAKE_SOURCE_DIR}/MiddleWare/HAL_Driver
    ${CMAKE_SOURCE_DIR}/MiddleWare/Interface
    ${CMAKE_SOURCE_DIR}/MiddleWare/Network
    ${CMAKE_SOURCE_DIR}/MiddleWare/Utils
    ${CMAKE_SOURCE_DIR}/Driver/esp32s3/esp32-camera
    ${CMAKE_SOURCE_DIR}/Driver/esp32s3/esp32_lcd
)

project(sparkbot_motion_control)
