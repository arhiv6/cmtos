#include <stddef.h>

#include "os.h"

struct os_t os;

void os_init()
{
    struct os_t empty = {0};
    os = empty;
}

bool os_add_task(task_function_t function, uint8_t priority, void *data)
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
    new_task.priority = priority;
    new_task.cost = 0;
    LC_INIT(&new_task.lc_state);

    os.tasks[os.task_count] = new_task;
    os.task_count++;

    return true;
}

void os_start()
{
    while (true)                                    // пока не выполним всё
    {
        uint32_t max_priority_task_number = 0;
        uint32_t max_cost = 0;
        for (uint8_t i = 0; i < os.task_count; i++) // проходим по списку задач
        {
            if (os.tasks[i].state == ready)
            {
                os.tasks[i].cost += os.tasks[i].priority;

                if ((os.tasks[i].cost > max_cost) ||
                        (os.tasks[i].cost == max_cost &&
                         os.tasks[i].priority > os.tasks[max_priority_task_number].priority))
                {
                    max_cost = os.tasks[i].cost;
                    max_priority_task_number = i;
                }
            }
        }
        os.current_task = &os.tasks[max_priority_task_number];
        if (os.current_task->state == ready)
        {
            os.current_task->cost = 0;
            os.current_task->function(os.current_task->data);
        }
    }
}
