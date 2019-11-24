#include <setjmp.h>

#include "test.h"
#include "os.h"

static void test_task(void *data)
{
    os_task_begin();
    os_yield();
    os_task_end();
}

static void test_task_exit(void *data)
{
    jmp_buf *quit = (jmp_buf *)data;
    longjmp(*quit, 0);
}

void test_os(void)
{
    jmp_buf quit;

    os_init();
    os_add_task(test_task, 0);
    os_add_task(test_task_exit, &quit);

    if (setjmp(quit))
    {
        return;
    }

    os_start();
}
