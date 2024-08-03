#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "../STHR.h"

static bool test_success;
static int print_level;
#define INDENT "    "
#define TEST_PASS 0
#define TEST_FAIL 1

void print_level_open(const char *string)
{
    int level = print_level;
    while (level > 0)
    {
        printf(INDENT);
        level--;
    }
    printf("%s:\n", string);
    print_level++;
}

void print_level_close(const char *string)
{
    print_level--;
    int level = print_level;
    while (level > 0)
    {
        printf(INDENT);
        level--;
    }
    printf("%s\n", string);
}

void test_pass_routine(void)
{
    print_level_close("PASS");
}

void test_fail_routine(void)
{
    test_success = false;
    print_level_close("FAIL");
}

void assert_result_ok(STHR_result_t result)
{
    if (STHR_OK == result)
        test_pass_routine();
    else
        test_fail_routine();
}

void assert_array_equal(int expected[], int actual[], size_t num_elements)
{
    if (0 != memcmp(expected, actual, num_elements * sizeof(int)))
        test_fail_routine();
    else
        test_pass_routine();
}

STHR_function_t thread_1_function;
STHR_function_t thread_2_function;

#define ARRAY_SIZE 10
static int array[ARRAY_SIZE];
static size_t array_index;
STHR_mutex_t mutex;

int main(void)
{
    print_level_open("Complete test");
    test_success = true;

    print_level_open("Mutex create");
    memset(array, 0, sizeof(array));
    array_index = 0;
    assert_result_ok(STHR_mutex_create(&mutex));

    print_level_open("Thread 1 create");
    STHR_thread_t thread_1;
    assert_result_ok(STHR_thread_create(&thread_1, thread_1_function));

    print_level_open("Thread 2 create");
    STHR_thread_t thread_2;
    assert_result_ok(STHR_thread_create(&thread_2, thread_2_function));

    print_level_open("Thread 1 wait end");
    assert_result_ok(STHR_thread_wait_end(&thread_1));

    print_level_open("Main mutex lock");
    assert_result_ok(STHR_mutex_lock(&mutex));

    print_level_open("Thread 2 destroy");
    assert_result_ok(STHR_thread_destroy(&thread_2));

    array[array_index++] = 3;

    print_level_open("Main mutex unlock");
    assert_result_ok(STHR_mutex_unlock(&mutex));

    print_level_open("Main mutex destroy");
    assert_result_ok(STHR_mutex_destroy(&mutex));

    print_level_open("Final array check");
    int expected[ARRAY_SIZE] = {1, 2, 1, 1, 2, 3, 0, 0, 0, 0};
    assert_array_equal(expected, array, ARRAY_SIZE);

    if (test_success)
    {
        test_pass_routine();
        return TEST_PASS;
    }
    test_fail_routine();
    return TEST_FAIL;
}

void thread_1_function(void)
{
    STHR_sleep_ms(500);

    print_level_open("Thread 1 mutex lock");
    assert_result_ok(STHR_mutex_lock(&mutex));

    array[array_index++] = 1;

    print_level_open("Thread 1 mutex unlock");
    assert_result_ok(STHR_mutex_unlock(&mutex));

    STHR_sleep_ms(1000);

    print_level_open("Thread 1 mutex lock");
    assert_result_ok(STHR_mutex_lock(&mutex));

    array[array_index++] = 1;

    STHR_sleep_ms(1000);

    array[array_index++] = 1;

    print_level_open("Thread 1 mutex unlock");
    assert_result_ok(STHR_mutex_unlock(&mutex));

    STHR_sleep_ms(500);
}

void thread_2_function(void)
{
    while (true)
    {
        STHR_sleep_ms(1000);

        print_level_open("Thread 2 mutex lock");
        assert_result_ok(STHR_mutex_lock(&mutex));

        array[array_index++] = 2;

        print_level_open("Thread 2 mutex unlock");
        assert_result_ok(STHR_mutex_unlock(&mutex));
    }
}