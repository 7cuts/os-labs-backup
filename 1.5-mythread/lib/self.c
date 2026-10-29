#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>

#include "my_thread_impl.h"
#include "thread_list.h"
#include "my_thread.h"

mythread *
mythread_self_tcb() {
    mythread* self = NULL;
    ll_lock();
    self = ll_find_by_tid(gettid());
    assert(self != NULL);
    assert(self->tid == gettid());
    ll_unlock();
    return self;
}

mythread_t
mythread_self()
{
    return (mythread_t) mythread_self_tcb();
}
