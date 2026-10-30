#include "my_thread_impl.h"
#include <stdbool.h>

void ll_lock();
void ll_unlock();

void ll_add(mythread *tcb);

void ll_remove(mythread *tcb);
bool ll_find(mythread *t);

bool ll_empty();

mythread *ll_find_by_tid(pid_t tid);
