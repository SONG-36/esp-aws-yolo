#include "task_entry.h"
#include "task_manager.h"
#include "display_task.h"
#include "wifi_softap_task.h" 

static const task_entry_t app_task_table[] = {
    { "display_task", display_task, 4096, 5 },
    { "hotspot_task", hotspot_task, 4096, 5 },
};

void register_all_tasks(void) {
    task_manager_register_table(app_task_table, sizeof(app_task_table)/sizeof(task_entry_t));
}
