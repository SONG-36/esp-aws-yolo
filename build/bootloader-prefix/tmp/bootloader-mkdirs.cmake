# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/andy/esp-idf-v5.3.2/components/bootloader/subproject"
  "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader"
  "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix"
  "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix/tmp"
  "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix/src/bootloader-stamp"
  "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix/src"
  "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/andy/esp32code/esp_sparkbot/example/tank/sparkbot_motion_control/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
