#include <setjmp.h>

#include "test.h"
#include "os.h"

enum state_t
{
    STATE_ZERO = 0,
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
};

enum state_t states[10] = {0};

static int state_num = 0;

static void test_task(void *data)
{
    os_task_begin();
    states[state_num++] = STATE_1;
    os_yield();
    states[state_num++] = STATE_2;
    os_task_end();
}

static void test_task_exit(void *data)
{
    states[state_num++] = STATE_3;
    jmp_buf *quit = (jmp_buf *)data;
    longjmp(*quit, 0);
}

void test_os(void)
{
    jmp_buf quit;
    states[state_num++] = STATE_0;

    os_init();

    os_add_task(test_task, 1, 0);
    os_add_task(test_task_exit, 2, &quit);

    if (setjmp(quit))
    {
        int i=0;
        TEST_EQ(states[i++], STATE_0);
        TEST_EQ(states[i++], STATE_3);
        TEST_EQ(states[i++], STATE_ZERO);

        return;
    }

    os_start();
}
