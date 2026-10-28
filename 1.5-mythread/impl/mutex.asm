	.file	"mutex.c"
	.text
	.type	my_syscall, @function
my_syscall:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%r8, -56(%rbp)
	movq	%r9, -64(%rbp)
	movq	-56(%rbp), %r10
	movq	-64(%rbp), %r8
	movq	16(%rbp), %r9
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdi
	movq	-40(%rbp), %rsi
	movq	-48(%rbp), %rdx
#APP
# 18 "mysyscall.h" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	my_syscall, .-my_syscall
	.globl	MUTEX_UNLOCKED
	.section	.rodata
	.align 4
	.type	MUTEX_UNLOCKED, @object
	.size	MUTEX_UNLOCKED, 4
MUTEX_UNLOCKED:
	.zero	4
	.globl	MUTEX_LOCKED
	.align 4
	.type	MUTEX_LOCKED, @object
	.size	MUTEX_LOCKED, 4
MUTEX_LOCKED:
	.long	1
	.text
	.type	futex, @function
futex:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movl	-16(%rbp), %ecx
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	pushq	$0
	movl	$0, %r9d
	movl	$0, %r8d
	movq	%rax, %rsi
	movl	$202, %edi
	call	my_syscall
	addq	$8, %rsp
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	futex, .-futex
	.globl	mythread_mutex_lock
	.type	mythread_mutex_lock, @function
mythread_mutex_lock:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -24(%rbp)
.L11:
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movl	$1, %eax
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %edx
	movq	-8(%rbp), %rcx
	movl	MUTEX_UNLOCKED(%rip), %eax
	lock cmpxchgl	%edx, (%rcx)
	movl	%eax, %edx
	sete	%al
	testb	%al, %al
	jne	.L6
	movl	%edx, MUTEX_UNLOCKED(%rip)
.L6:
	testb	%al, %al
	jne	.L13
	movl	$1, %edx
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	futex
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	jns	.L11
	cmpl	$-11, -12(%rbp)
	je	.L11
	movl	-12(%rbp), %eax
	jmp	.L10
.L13:
	nop
	movl	$0, %eax
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	mythread_mutex_lock, .-mythread_mutex_lock
	.globl	mythread_mutex_unlock
	.type	mythread_mutex_unlock, @function
mythread_mutex_unlock:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rbx
	call	mythread_self
	cmpq	%rax, %rbx
	je	.L15
	movl	$-1, %eax
	jmp	.L14
.L15:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 8(%rax)
.L14:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	mythread_mutex_unlock, .-mythread_mutex_unlock
	.globl	mythread_mutex_init
	.type	mythread_mutex_init, @function
mythread_mutex_init:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	mythread_mutex_init, .-mythread_mutex_init
	.ident	"GCC: (GNU) 16.2.1 20260819 (Red Hat 16.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
