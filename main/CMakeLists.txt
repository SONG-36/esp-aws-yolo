idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "../MiddleWare/Core/include"
                    PRIV_REQUIRES Core App HAL_Driver esp32-camera esp32_lcd Interface Utils
                    )
