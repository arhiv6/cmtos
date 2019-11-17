#include <stdio.h>
#include <stdlib.h>

#include "test.h"

#define COLOR_RED(TEXT)     "\033[1;31m" TEXT "\033[0m"
#define COLOR_GREEN(TEXT)   "\033[1;32m" TEXT "\033[0m"

struct test_internal_data test;

void test_init()
{
    static struct test_internal_data zero = {0};
    test = zero;
    printf("Running tests.\n");
}

void test_run_suite(const char *name, void (*function)(void))
{
    printf("\r\nRun suite %s:\n", name);
    fflush(stdout);
    test.number_in_suite = 0;
    function();
}

void test_log(const char *function, char *file, unsigned int line, bool state)
{
    test.number_in_suite++;

    if (state)
    {
        test.pass_counter++;
        printf(COLOR_GREEN(" + "));
    }
    else
    {
        test.fail_counter++;
        printf(COLOR_RED(" - "));
    }

    printf("test %2u  %s\n", test.number_in_suite, test.text);

    if (!state)
    {
        printf("  %s:%u, function %s\n", file, line, function);
    }

    fflush(stdout);
}

int test_report()
{
    printf("\r\n");
    printf("Runs:   %u\n", test.pass_counter + test.fail_counter);
    printf("Passes: %u\n", test.pass_counter);
    printf("Fails:  %u\n", test.fail_counter);
    printf("Status:");

    if (test.fail_counter)
    {
        printf(COLOR_RED(" FAIL \n\n"));
        return EXIT_FAILURE;
    }
    else
    {
        printf(COLOR_GREEN(" PASSED \n\n"));
        return EXIT_SUCCESS;
    }
}
