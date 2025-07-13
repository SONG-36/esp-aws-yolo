#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "task_entry.h"
#include "task_manager.h"

extern void register_all_tasks(void);

void app_main(void) {
    register_all_tasks();               // 👈 App 注册任务表给 Core
    task_registry_init();              // 👈 Core 遍历任务表
}