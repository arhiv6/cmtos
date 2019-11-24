#ifndef OS_H
#define OS_H

#include  <stdbool.h>
#include  <stdint.h>

#include  "os_lc.h"

typedef enum
{
    not_init,
    ready,
    paused,
} task_state_t;

typedef void (*task_function_t)(void *);

typedef struct
{
    task_function_t function;
    lc_t lc_state;
    task_state_t state;
    uint8_t priority;
    uint8_t cost;
    void *data;
} task_t;

struct os_t
{
    uint32_t task_count;
    task_t tasks[OS_MAX_TASKS];
    task_t *current_task;
};

extern struct os_t os;

//---------------------------------------------

// necessary prologue of the task
#define os_task_begin()     LC_BEGIN(&os.current_task->lc_state)

// pass control to the next ready task
#define os_yield()          LC_YIELD(&os.current_task->lc_state)

// necessary epilogue of the task
#define os_task_end()       LC_END(&os.current_task->lc_state)

//---------------------------------------------

void os_init();
bool os_add_task(task_function_t function, uint8_t priority, void *data);
void os_start();

#endif // OS_H
