#include "my_syscall.h"
#include "my_thread_impl.h"

void
mythread_exit(void *retval)
{
    mythread *self = mythread_self_tcb();

    self->retval = retval;

    // Kernel will set tcb->tid to zero and do futex_wake
    my_syscall1(SYS_exit, 0);
}
