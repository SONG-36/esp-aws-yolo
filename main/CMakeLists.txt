idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "../MiddleWare/Core/include"
                    PRIV_REQUIRES Core nvs_flash
                    )

# spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)

# if(EXISTS ../web_remote_control/dist/index.html)
#     spiffs_create_partition_image(
#         webserver
#         "../web_remote_control/dist"
#         FLASH_IN_PROJECT
#     )
# else()
#     message(FATAL_ERROR "../web_remote_control/dist doesn't exit. Please run 'yarn build' in ../web_remote_control/")
# endif()