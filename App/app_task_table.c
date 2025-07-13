#include "task_entry.h"
#include "task_manager.h"
#include "helloworld_task.h"

static const task_entry_t app_task_table[] = {
    { "HelloTask", hello_task, 2048, 5 },
};

void register_all_tasks(void) {
    task_manager_register_table(app_task_table, sizeof(app_task_table)/sizeof(task_entry_t));
}
