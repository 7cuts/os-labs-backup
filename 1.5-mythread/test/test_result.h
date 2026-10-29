#ifndef TEST_RESULT_H
#define TEST_RESULT_H

#include <stdbool.h>
typedef struct test_result_t {
    bool ok;
    char message[256];
} test_result_t;


#define FAIL(FORMAT, ...) \
    test_result_t __res;  \
    __res.ok = false;     \
    snprintf(__res.message, sizeof(__res.message),  \
        "%s: "FORMAT"\n", __FUNCTION__, __VA_ARGS__);                   \
    return __res

#define OK_MSG(FORMAT, ...) \
    test_result_t __ok_res;  \
    __ok_res.ok = true;     \
    snprintf(__ok_res.message, sizeof(__ok_res.message),  \
        "%s: "FORMAT"\n", __FUNCTION__, __VA_ARGS__);                   \
    return __ok_res

#define OK \
    test_result_t __success;  \
    __success.ok = true;     \
    snprintf(__success.message, sizeof(__success.message),  \
        "%s\n", __FUNCTION__);                   \
    return __success

#define TEST(func_name) test_result_t func_name()
#endif
