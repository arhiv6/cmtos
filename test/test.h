#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_STRLEN 128

struct test_internal_data
{
    unsigned int pass_counter;
    unsigned int fail_counter;
    unsigned int number_in_suite;
    char text[MAX_STRLEN];
};

extern struct test_internal_data test;

#define TEST_EQ(expected, result) do {                                      \
    snprintf(test.text, MAX_STRLEN, "TEST_EQ(%s, %s)", #expected, #result); \
    test_log(__FUNCTION__, __FILE__, __LINE__, (expected) == (result));     \
} while(0)

void test_init();
void test_run_suite(const char *name, void (*function)(void));
void test_log(const char *function, char *file, unsigned int line, bool state);
int test_report();

#endif /* TEST_H */
