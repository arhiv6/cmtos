#include "test.h"

extern void test_os_lc(void);

int main(void)
{
    test_init();

    test_run_suite("test_os_lc", test_os_lc);

    return test_report();
}

