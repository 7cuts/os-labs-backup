#ifndef MYTHREAD_IMPLE_H
#define MYTHREAD_IMPLE_H

/* Public definitions */
#include "../mythread.h"

typedef enum {
    MYTHREAD_RUNNING,
    MYTHREAD_JOINED,
    MYTHREAD_DEAD
} mythread_state;

typedef struct mythread {
    struct mythread *self; /* `self` is set by kernel with CLONE_SETTLS */
    mythread_t mythread_id;
    void* arg;
    void* (*start_routine)(void*);
    void* retval;
    mythread_state state;
    int local_errno;
} mythread;

mythread* mythread_self_tcb();

#endif
