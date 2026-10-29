#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>

#include "thread_list.h"
#include "my_thread.h"
#include "my_mutex.h"

static mythread_mutex_t ll_mutex;
static mythread main_tcb; // for "main" thread
static mythread *head;
static mythread *tail;

void
ll_lock() {
    int err = mythread_mutex_lock(&ll_mutex);
    assert(err == 0);
}

void
ll_unlock() {
    int err = mythread_mutex_unlock(&ll_mutex);
    assert(err == 0);
}

void
ll_add(mythread *tcb)
{
    if (!tail) {
        // first time calling it. we are in main thread.
        assert(!head);
        assert(gettid() == getpid());
        head = &main_tcb;
        tail = head;
        main_tcb.tid = gettid();
    }

    // add new thread structure to the list of threads.
    assert(!tail->next);
    tail->next = tcb;
    tcb->prev = tail;
    tail = tcb;
}

bool
ll_empty()
{
    return head == NULL;
}

void
ll_remove(mythread *tcb)
{
    if (tcb == NULL) return;

    if (!tcb->next && !tcb->prev) {
        assert(0);
        // it is strange .....
        return;
    }

    if (!tcb->prev) {
        /* it is the head */
        tcb->next->prev = NULL;
        head = tcb->next;
        return;
    }

    if (!tcb->next) {
        /* it is the tail*/
        tcb->prev->next = NULL;
        tail = tcb->prev;
        return;
    }

    /* nor head nor tail. we're in the middle */

    tcb->prev->next = tcb->next;
    tcb->next->prev = tcb->prev;

    tcb->next = NULL;
    tcb->prev = NULL;
}

bool
ll_find(mythread *t)
{
    for (mythread* p = head; p != NULL; p = p->next) {
        if (p == t) {
            return true;
        }
    }
    return false;
}

mythread *
ll_find_by_tid(pid_t tid)
{
    for (mythread* p = head; p != NULL; p = p->next) {
        if (p->tid == tid) {
            return p;
        }
    }
    return NULL;
}
