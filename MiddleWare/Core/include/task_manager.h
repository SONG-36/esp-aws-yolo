#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <stdint.h>
#include "task_entry.h"

#ifdef __cplusplus
extern "C" {
#endif

// 核心初始化函数
void task_registry_init(void);  // 调度入口，遍历任务表并创建任务

// 任务表的外部声明
void task_manager_register_table(const task_entry_t* table, size_t count);

#ifdef __cplusplus
}
#endif

#endif