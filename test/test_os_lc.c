#include "test.h"
#include "os_lc.h"

static enum
{
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4,
} state;

static void test_task(lc_t *lc)
{
    state = STATE_0;
    LC_BEGIN(lc);    
    state = STATE_1;
    LC_YIELD(lc);
    state = STATE_2;
    LC_YIELD(lc);
    state = STATE_3;
    LC_END(lc);
    state = STATE_4;
}

void test_os_lc(void)
{
    lc_t lc;
    state = STATE_0;

    // начальная инициализация не меняет состояние
    LC_INIT(&lc);
    TEST_EQ(state, STATE_0);

    // первый вызов функции измененит состояние и приостановит её работу
    test_task(&lc);
    TEST_EQ(state, STATE_1);

    // повторная инициализация и первый вызов приведёт к тому же состоянию
    LC_INIT(&lc);
    test_task(&lc);
    TEST_EQ(state, STATE_1);

    // ещё один вызов приведёт к следующему состоянию
    test_task(&lc);
    TEST_EQ(state, STATE_2);

    // завершение произошло с помощью LC_END а не по выходу из функции
    test_task(&lc);
    TEST_EQ(state, STATE_3);

     // автоматическая реинициализация после завершения с помощью LC_END
    test_task(&lc);
    TEST_EQ(state, STATE_1);
}
