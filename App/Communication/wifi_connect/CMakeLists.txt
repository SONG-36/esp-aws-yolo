idf_component_register(
    SRCS "wifi_softap_task.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES freertos Core web_server
    REQUIRES esp_wifi esp_event nvs_flash web_server
)