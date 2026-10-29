
#include <stdio.h>
#include "test_result.h"

extern test_result_t test_arg(void);
extern test_result_t test_basic(void);
extern test_result_t test_many(void);
extern test_result_t test_data_race(void);
extern test_result_t test_join_wrong_id(void);
extern test_result_t test_join_self(void);
extern test_result_t test_join_self_v2(void);
extern test_result_t test_mutex(void);
extern test_result_t test_mutex_double_lock(void);
extern test_result_t test_mutex_bad_initialization(void);
extern test_result_t test_exit(void);
extern test_result_t test_self(void);
extern test_result_t test_cancel(void);
extern test_result_t test_cancel_wrong_id(void);
extern test_result_t test_cancel_async(void);
extern test_result_t test_cancel_self(void);
extern test_result_t test_cancel_async_self(void);

#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

bool do_test(test_result_t (*func)(void)) {
    test_result_t res = func();
    printf("[%4s] %s", res.ok ? GREEN"OK"RESET : RED"FAIL"RESET, res.message);
    return res.ok;
}

int
main(int argc, char *argv[])
{
    do_test(test_basic);
    do_test(test_arg);
    do_test(test_many);
    do_test(test_data_race);
    do_test(test_join_wrong_id);
    do_test(test_join_self);
    do_test(test_join_self_v2);
    do_test(test_mutex);
    do_test(test_mutex_double_lock);
    do_test(test_mutex_bad_initialization);
    do_test(test_exit);
    do_test(test_self);
    do_test(test_cancel);
    do_test(test_cancel_self);
    do_test(test_cancel_wrong_id);
    do_test(test_cancel_async);
    do_test(test_cancel_async_self);
}
