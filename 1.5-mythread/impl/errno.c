#include "../myerrno.h"

#include "mythread_impl.h"

int *
my_errno_location() {
    return &mythread_self_tcb()->local_errno;
}
