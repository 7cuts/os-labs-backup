#include <asm-generic/errno-base.h>
#include <unistd.h>
#include <setjmp.h>

#include "my_thread_impl.h"
#include "my_thread.h"
#include "thread_list.h"
#include "my_syscall.h"

void
async_cancel_handler(int signo, siginfo_t *info, void *uctx)
{
    if (signo != SIGUSR1) {
        return;
    }

    if (info->si_pid != getpid()) {
        return;
    }

    /* TODO: fix this thing */
    mythread *self = mythread_self_tcb();
    siglongjmp(self->env, 0);
}


int
mythread_cancel(mythread_t thread)
{
    mythread *tcb = (mythread *) thread;

    ll_lock();
    bool found = ll_find(tcb);
    ll_unlock();

    if (!found) {
        return -ESRCH;
    }

    tcb->canceled = true;
    return 0;
}

void
mythread_testcancel(void)
{
    mythread *self = mythread_self_tcb();
    if (self->canceled) {
        longjmp(self->env, 0);
    }
}


int
mythread_cancel_async(mythread_t thread)
{
    mythread *tcb = (mythread *) thread;

    ll_lock();
    bool found = ll_find(tcb);
    ll_unlock();

    if (!found) {
        return -ESRCH;
    }

    tcb->canceled = true;

    my_syscall3(SYS_tgkill, getpid(), tcb->tid, SIGUSR1);
    return 0;
}
