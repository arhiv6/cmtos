#include "test.h"
#include "mpool.h"

typedef long int test_type;
static test_type test_memory[4];

static mpool_t pool;
static test_type *p1, *p2, *p3, *p4, *p5;

void test_mpool0(void)
{
    mpool_t zero_pool = {};
    pool = zero_pool;

    mp_init((mpool_t *)NULL, sizeof(test_memory), sizeof(test_type), test_memory);
    TEST_EQ(pool.head, zero_pool.head);
    mp_init(&pool, (size_t)NULL, sizeof(test_type), test_memory);
    TEST_EQ(pool.head, zero_pool.head);
    mp_init(&pool, sizeof(test_memory), sizeof(test_type), (void *)NULL);
    TEST_EQ(pool.head, zero_pool.head);

    mp_init(&pool, sizeof(test_memory), sizeof(test_type), test_memory);
    TEST_NE(pool.head, zero_pool.head);

    p5 = mp_alloc(NULL);
    TEST_EQ(p5, NULL);
}

void test_mpool1(void)
{
    p1 = mp_alloc(&pool);
    TEST_NE(p1, NULL);

    p2 = mp_alloc(&pool);
    TEST_NE(p2, NULL);

    p3 = mp_alloc(&pool);
    TEST_NE(p3, NULL);

    p4 = mp_alloc(&pool);
    TEST_NE(p4, NULL);

    p5 = mp_alloc(&pool); // oversize
    TEST_EQ(p5, NULL);
}

void test_mpool2(void)
{
    *p1 = 1;
    *p2 = 2;
    *p3 = 3;
    *p4 = 4;

    TEST_EQ(*p1, 1);
    TEST_EQ(*p2, 2);
    TEST_EQ(*p3, 3);
    TEST_EQ(*p4, 4);
}

void test_mpool3(void)
{
    mp_free(&pool, p1);
    mp_free(&pool, p4);

    p5 = mp_alloc(&pool);
    TEST_NE(p5, NULL);
    *p5 = 5;

    TEST_EQ(*p2, 2);
    TEST_EQ(*p3, 3);
    TEST_EQ(*p5, 5);
}

void test_mpool4()
{
    TEST_ABORT_SECTION
    {
        mp_init((mpool_t *)NULL, (size_t)NULL, (size_t)NULL, (void *)NULL);
    };

    TEST_ABORT_SECTION
    {
        mp_init((mpool_t *)NULL, (size_t)NULL, (size_t)NULL, (void *)NULL);
    };
}

void test_mpool(void)
{
    p1 = p2 = p3 = p4 = p5 = NULL;

    test_mpool0();
    test_mpool1();
    test_mpool2();
    test_mpool3();
    test_mpool4();
}
