idf_component_register(
    SRCS "app_task_table.c" "helloworld_task.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES freertos
    PRIV_REQUIRES Core
)
