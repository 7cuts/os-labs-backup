#ifndef MY_THREAD_IMPL_H
#define MY_THREAD_IMPL_H

#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>

typedef struct mythread mythread;

struct mythread {
    mythread *next;
    mythread *prev;

    void *arg;
    void *(*start_routine)(void *);
    void *retval;

    _Atomic(uint32_t) state;
    pid_t tid;

    void *map_base;
    size_t map_size;

    sigset_t init_mask;
    jmp_buf env;

    bool canceled;
};

mythread *mythread_self_tcb();

void async_cancel_handler(int signo, siginfo_t *info, void *uctx);

#endif
