#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "task_entry.h"     // 包含任务结构体定义
#include "task_manager.h"   // 包含函数声明



static const task_entry_t* internal_task_table = NULL;
static size_t internal_task_count = 0;


void task_manager_register_table(const task_entry_t* table, size_t count) {
    internal_task_table = table;
    internal_task_count = count;
}

// 遍历 internal_task_table 并创建任务
void task_registry_init(void) {
    if (!internal_task_table || internal_task_count == 0) {
        printf("No task table registered!\n");
        return;
    }

    for (size_t i = 0; i < internal_task_count; ++i) {
        const task_entry_t* task = &internal_task_table[i];
        printf("[task_manager] Creating task: %s\n", task->task_name);
        xTaskCreate(task->task_entry, task->task_name, task->stack_size, NULL, task->priority, NULL);
    }
}
