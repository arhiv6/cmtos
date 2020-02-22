#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "test.h"

#ifdef _WIN32
#include <Windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

static HANDLE stdoutHandle;
static DWORD outModeInit;

static void enable_colors_in_console(void)
{
    stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD outMode = 0;
    GetConsoleMode(stdoutHandle, &outMode);

    outModeInit = outMode;
    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;  // Enable ANSI escape codes

    SetConsoleMode(stdoutHandle, outMode);
}

static void restore_colors_in_console(void)
{
    printf("\x1b[0m");                              // Reset colors
    SetConsoleMode(stdoutHandle, outModeInit);      // Reset console mode
}
#endif

#define COLOR_RED(TEXT)     "\033[1;31m" TEXT "\033[0m"
#define COLOR_GREEN(TEXT)   "\033[1;32m" TEXT "\033[0m"

struct test_internal_data test;

static void sigabrt_trap(int dummy)
{
    longjmp(test.sigabrt_jmp_buf, dummy);
}

void test_init()
{
    signal(SIGABRT, sigabrt_trap);
#ifdef _WIN32
    enable_colors_in_console();
#endif
    static struct test_internal_data zero = {0};
    test = zero;
    printf("Running tests.\n");
}

void test_run_suite(const char *name, void (*function)(void))
{
    printf("\r\nRun suite %s:\n", name);
    fflush(stdout);
    test.number_in_suite = 0;

    if (!setjmp(test.sigabrt_jmp_buf))    /* set abort() trap for assert() */
    {
        function();
    }
    else                            /* trap worked */
    {
        printf(COLOR_RED(" - ") "Suite aborted\r\n");
        test.fail_counter++;
    }
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
    printf("\nStatus:");

    int state;
    if (test.fail_counter)
    {
        printf(COLOR_RED(" FAIL \n\n"));
        state = EXIT_FAILURE;
    }
    else
    {
        printf(COLOR_GREEN(" PASSED \n\n"));
        state = EXIT_SUCCESS;
    }

#ifdef _WIN32
    restore_colors_in_console();
#endif
    return state;
}
