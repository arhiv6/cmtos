#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>

#define MAX_STRLEN 128

struct test_internal_data
{
    unsigned int pass_counter;
    unsigned int fail_counter;
    unsigned int number_in_suite;
    char text[MAX_STRLEN];
    jmp_buf sigabrt_jmp_buf;
    jmp_buf tmp_jmp_buf ;
};

extern struct test_internal_data test;

#define TEST_EQ(expected, result) do {                                      \
    snprintf(test.text, MAX_STRLEN, "TEST_EQ(%s, %s)", #expected, #result); \
    test_log(__FUNCTION__, __FILE__, __LINE__, (expected) == (result));     \
} while(0)

#define TEST_NE(expected, result) do {                                      \
    snprintf(test.text, MAX_STRLEN, "TEST_NE(%s, %s)", #expected, #result); \
    test_log(__FUNCTION__, __FILE__, __LINE__, (expected) != (result));     \
} while(0)

#define UNIC_CONCAT(unic_num)   _i_ ## unic_num
#define UNIC_VAR(unic_num)   UNIC_CONCAT(unic_num)

#define TEST_ABORT_SECTION                                                  \
    snprintf(test.text, MAX_STRLEN, "TEST_ABORT( )");                       \
    test.tmp_jmp_buf[0] = test.sigabrt_jmp_buf[0];                          \
    int UNIC_VAR(__LINE__)= setjmp(test.sigabrt_jmp_buf);                   \
    for ( (UNIC_VAR(__LINE__) == 0) ? 0 :                                   \
            ( test.sigabrt_jmp_buf[0] = test.tmp_jmp_buf[0],                \
            test_log(__FUNCTION__, __FILE__, __LINE__, true)) ;             \
         0 == UNIC_VAR(__LINE__)++;                                         \
         (UNIC_VAR(__LINE__ )!= 0) ?                                        \
            ( test.sigabrt_jmp_buf[0] = test.tmp_jmp_buf[0],                \
            test_log(__FUNCTION__, __FILE__, __LINE__, false)) :0  )        \

void test_init();
void test_run_suite(const char *name, void (*function)(void));
void test_log(const char *function, char *file, unsigned int line, bool state);
int test_report();

#endif /* TEST_H */
