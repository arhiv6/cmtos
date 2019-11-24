#include <stddef.h>

#include "os.h"

struct os_t os;

void os_init()
{
    struct os_t empty = {0};
    os = empty;
}

bool os_add_task(task_function_t function, void *data)
{
    if (function == NULL)
    {
        return false;
    }

    if (os.task_count > OS_MAX_TASKS)
    {
        return false;
    }

    task_t new_task = {0};
    new_task.function = function;
    new_task.state = ready;
    new_task.data = data;
    LC_INIT(&new_task.lc_state);

    os.tasks[os.task_count] = new_task;
    os.task_count++;

    return true;
}

void os_start()
{
    while (true)                                    // пока не выполним всё
    {
        for (uint8_t i = 0; i < os.task_count; i++) // проходим по списку задач
        {
            if (os.tasks[i].state == ready)
            {
                os.current_task = &os.tasks[i];
                os.current_task->function(os.current_task->data);
            }
        }
    }
}
