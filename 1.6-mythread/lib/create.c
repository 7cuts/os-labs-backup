#define _GNU_SOURCE
#include <sched.h>
#include "my_thread.h"
#include "my_thread_impl.h"
#include "thread_list.h"

#include <sys/mman.h>
#include "my_syscall.h"
#include <assert.h>
#include <asm-generic/errno-base.h>

#define STACK_SIZE 8 * 1024 * 1024 - sizeof(mythread)
#define GUARD_PAGE_SIZE 4096
#define THREAD_MEM (STACK_SIZE + sizeof(mythread) + GUARD_PAGE_SIZE)

static void
block_all_signals(sigset_t *old)
{
    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_SETMASK, &set, old);
}

static void
unblock_signals(sigset_t *old) {
    sigprocmask(SIG_SETMASK, old, NULL);
}

static const int flags = CLONE_VM | CLONE_FS | CLONE_THREAD | CLONE_FILES | CLONE_SIGHAND
    | CLONE_PARENT_SETTID   // set tcb->tid before child thread start
    | CLONE_CHILD_CLEARTID; // zero tcb->tid when child thread calls exit()

// Return NULL if cannot alloc
static void *
alloc_thread() {
    long res = my_syscall6(SYS_mmap, NULL, THREAD_MEM,
        PROT_READ | PROT_WRITE, MAP_STACK | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    assert(res != -EINVAL);

    if (res < 0) {
        return NULL;
    }

    return (void *)res;
}


static int
thread_start(void *arg) {

    mythread *tcb = (mythread *) arg;
    void *retval;

    // Save the signal mask. Currently all signals
    // are blocked. When thread returns here from
    // async cancellation signal handler, they will
    // have all blocked signals. That's what i need
    // to call mythread_exit() for this thread.
    int not_first_call = sigsetjmp(tcb->env, 0);

    if (!not_first_call) {
        // Unlock signals if thread just started
        sigprocmask(SIG_SETMASK, &tcb->init_mask, NULL);
        retval = tcb->start_routine(tcb->arg);
        mythread_exit(retval);
    }

    // We returned by jumping.
    mythread_exit(MYTHREAD_CANCELED);
}


int
mythread_create(mythread_t* thread, void *(*start_routine)(void*), void *arg) {
    int err;
    // 0x0 [  TCB|STACK|GUARD_PAGE  ]  0xF..F
    void *region = alloc_thread();

    if (!region) {
        return -EAGAIN;
    }

    mythread *tcb = region;
    *tcb = (mythread) {
        .arg = arg,
        .start_routine = start_routine,
        .map_base = region,
        .map_size = THREAD_MEM
    };


    ll_lock();
    block_all_signals(&tcb->init_mask);

    if (ll_empty()) {
        // We are in the main thread right now. It's
        // a good time to set up signal handler for
        // asynchronous cancel.

        sigset_t sigmask;
        sigfillset(&sigmask);
        struct sigaction act = {
            .sa_sigaction = async_cancel_handler,
            .sa_mask = sigmask, /* block all */
            .sa_flags = SA_SIGINFO,

        };

        sigaction(SIGUSR1, &act, NULL);
    }


    ll_add(tcb);

    // I use libc wrapper here because i don't want to deal
    // with filling stack for child process. And i don't care
    // about errno value because any error of clone we treat as
    // "not enough memory right now for create a new process".
    err = clone(thread_start, region + sizeof(mythread) + STACK_SIZE,
        flags, tcb, &tcb->tid /*parent_tid*/, NULL, &tcb->tid /*child_tid*/);

    ll_unlock();


    if (err == -1) {
        assert(err != -EINVAL);
        return -EAGAIN;
    }

    // tcb->tid now stores the child tid.
    *thread = (mythread_t )tcb;
    unblock_signals(&tcb->init_mask);
    return 0;
}
