file(GLOB_RECURSE NETWORK
    "src/*.c"
)

idf_component_register(
    SRCS ${NETWORK}
    INCLUDE_DIRS
        "include"
    REQUIRES esp_http_server esp_wifi nvs_flash spiffs esp_event esp_netif
)