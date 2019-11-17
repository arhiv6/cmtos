#include "test.h"

extern void test_os_lc(void);
extern void test_os(void);

int main(void)
{
    test_init();

    test_run_suite("test_os_lc", test_os_lc);
    test_run_suite("test_os", test_os);

    return test_report();
}

