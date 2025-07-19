file(GLOB_RECURSE UTILS
    "src/*.c"
)

idf_component_register(
    SRCS ${UTILS}
    INCLUDE_DIRS
        "include"
    REQUIRES driver spiffs
    PRIV_REQUIRES fatfs
)

set(COMPONENT_KCONFIG ${CMAKE_CURRENT_LIST_DIR}/Kconfig)