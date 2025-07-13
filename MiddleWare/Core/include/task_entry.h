#ifndef __TASK_ENTRY_H__
#define __TASK_ENTRY_H__

#include <stdint.h>
#include "freertos/FreeRTOS.h"

// 任务入口函数原型定义
typedef void (*task_func_t)(void*);

// 单个任务配置结构体
typedef struct {
    const char*    task_name;     // 任务名称
    task_func_t    task_entry;    // 任务入口函数
    uint16_t       stack_size;    // 栈大小（单位：word）
    UBaseType_t    priority;      // 优先级
} task_entry_t;



#endif  // __TASK_ENTRY_H__
