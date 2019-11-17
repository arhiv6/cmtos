#define OS_LC_USE_GOTO
#include "os_lc.h"

#include "test.h"

static void test_task_a(lc_t *lc, int *val)
{
    LC_BEGIN(lc);
    *val += 1;
    LC_YIELD(lc);
    *val += 3;
    LC_END();
}

static void test_task_b(lc_t *lc, int *val)
{
    LC_BEGIN(lc);
    *val += 5;
    LC_YIELD(lc);
    *val += 7;
    LC_END();
}

void test_os_lc(void)
{
    lc_t a, b;

    int val1 = 0;
    LC_INIT(&a);
    test_task_a(&a, &val1);
    TEST_EQ(val1, 1);
    val1 = 0;
    LC_INIT(&a);
    test_task_a(&a, &val1);
    TEST_EQ(val1, 1);

    val1 = 0;
    int val2 = 0;
    LC_INIT(&a);
    LC_INIT(&b);
    test_task_a(&a, &val1);
    val2 += 1;
    TEST_EQ(val1, val2);
    test_task_b(&b, &val1);
    val2 += 5;
    TEST_EQ(val1, val2);
    test_task_a(&a, &val1);
    val2 += 3;
    TEST_EQ(val1, val2);
    test_task_b(&b, &val1);
    val2 += 7;
    TEST_EQ(val1, val2);
}
