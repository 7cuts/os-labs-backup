#include <linux/futex.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>

#include "my_thread.h"
#include "my_thread_impl.h"
#include "thread_list.h"
#include "my_syscall.h"

#include <errno.h>
#undef errno

int
mythread_join(mythread_t thread, void** retval)
{
    // Kernel will set tcb->pid to zero and
    // do futex_wake when thread exited.

    mythread *tcb = (mythread *) thread;

    // We should not join the calling thread.
    if (tcb->tid == gettid()) {
        // tcb->tid cannot be zero if it is our thread's tcb, because
        // kernel sets tcb->tid to 0 only when the thread is exiting.
        // if we're here, our thread has not exited yet.
        return -EDEADLK;
    }


    // Check if `thread` is a valid thread ID.
    ll_lock();
    bool found = ll_find(tcb);
    ll_unlock();
    if (!found) {
        return -ESRCH;
    }

    // Wait on tid untill it does not equal to zero.
    while (1) {
        pid_t tid = tcb->tid;
        if (tid == 0) {
            break;
        }

        int r = futex((uint32_t*) &tcb->tid, FUTEX_WAIT, tid);

        assert(r == 0 || r == -EINVAL || r == -EINTR);
    }

    if (retval) {
        *retval = tcb->retval;
    }

    ll_lock();
    ll_remove(tcb);
    ll_unlock();

    // Deallocate thread's memory
    my_syscall2(SYS_munmap, tcb->map_base, tcb->map_size);
    return 0;
}
