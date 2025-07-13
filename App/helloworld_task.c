#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void hello_task(void* arg) {
    while (1) {
        printf("Hello World!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}