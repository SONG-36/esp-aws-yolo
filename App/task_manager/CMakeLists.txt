idf_component_register(
    SRCS "app_task_table.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES freertos Core display_output wifi_connect
)
