
#ifndef _SYSCALL_H
#define _SYSCALL_H 1

/* Include __NR_* consts */
#include <asm/unistd.h>

/* Include SYS_* constants based on __NR_* */
#include <bits/syscall.h>

static inline long
my_syscall(long syscall_no, long a1, long a2, long a3,
                                  long a4, long a5, long a6) {
    long result;
    register long r10 asm("r10") = a4;
    register long r8  asm("r8")  = a5;
    register long r9  asm("r9")  = a6;
    asm volatile (
        "syscall"
        : "=a" (result)
        : "a" (syscall_no), "D" (a1), "S" (a2), "d" (a3), "r" (r10), "r" (r8), "r" (r9)
        : "rcx", "r11", "memory"
    );
    return result;
}

#define my_syscall1(syscall_no, a1) \
    my_syscall((long)syscall_no, (long)(a1), 0, 0, 0, 0, 0)

#define my_syscall2(syscall_no, a1, a2) \
    my_syscall((long)syscall_no, (long)(a1), (long)(a2), 0, 0, 0, 0)

#define my_syscall3(syscall_no, a1, a2, a3) \
    my_syscall((long)syscall_no, (long)(a1), (long)(a2), (long)(a3), 0, 0, 0)

#define my_syscall4(syscall_no, a1, a2, a3, a4) \
    my_syscall((long)syscall_no, (long)(a1), (long)(a2), (long)(a3), (long)(a4), 0, 0)

#define my_syscall5(syscall_no, a1, a2, a3, a4, a5) \
    my_syscall((long)syscall_no, (long)(a1), (long)(a2), (long)(a3), (long)(a4), (long)(a5), 0)

#define my_syscall6(syscall_no, a1, a2, a3, a4, a5, a6) \
    my_syscall((long)syscall_no, (long)(a1), (long)(a2), (long)(a3), (long)(a4), (long)(a5), (long)(a6))

#endif
