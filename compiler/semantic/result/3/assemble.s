.LC0:
	.string	"%d"
.LC1:
	.string	"%d\n"
	.section	.rodata
	.align	4
	.type	N, @object
	.size	N, 4
N:
	.long	10
	.text
	.globl	a
	.data
	.align	32
	.type	a, @object
	.size	a, 40
a:
	.zero	40
	.text
	.globl	b
	.data
	.align	32
	.type	b, @object
	.size	b, 40
b:
	.zero	40
	.text
	.globl	c
	.data
	.align	32
	.type	c, @object
	.size	c, 40
c:
	.zero	40
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$4, %rsp
	subq	$4, %rsp
	subq	$8, %rsp
	leaq	-4(%rbp), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	leaq	-8(%rbp), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	$0, %edi
	subq	$4, %rsp
	movl	%edi, -20(%rbp)
.L1:
	subq	$12, %rsp
.L2:
	movl	-20(%rbp), %r8d
	movl	-4(%rbp), %r9d
	cmpl	%r9d, %r8d
	jl	.L4
	jge	.L3
.L3:
	addq	$12, %rsp
	jmp	.L5
.L4:
	subq	$4, %rsp
	movl	$0, -36(%rbp)
	movl	-20(%rbp), %r8d
	imull	$1, %r8d
	addl	-36(%rbp), %r8d
	movl	%r8d, -36(%rbp)
	subq	$12, %rsp
	movl	-36(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	a(%rip), %rbx
	leaq	(%rdx, %rbx), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-20(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -52(%rbp)
	movl	-52(%rbp), %r9d
	movl	%r9d, -20(%rbp)
	addq	$32, %rsp
	jmp	.L1
.L5:
	movl	$0, %r9d
	movl	%r9d, -20(%rbp)
.L6:
	subq	$12, %rsp
.L7:
	movl	-20(%rbp), %r8d
	movl	-8(%rbp), %r9d
	cmpl	%r9d, %r8d
	jl	.L9
	jge	.L8
.L8:
	addq	$12, %rsp
	jmp	.L10
.L9:
	subq	$4, %rsp
	movl	$0, -36(%rbp)
	movl	-20(%rbp), %r8d
	imull	$1, %r8d
	addl	-36(%rbp), %r8d
	movl	%r8d, -36(%rbp)
	subq	$12, %rsp
	movl	-36(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	b(%rip), %rbx
	leaq	(%rdx, %rbx), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-20(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -52(%rbp)
	movl	-52(%rbp), %r9d
	movl	%r9d, -20(%rbp)
	addq	$32, %rsp
	jmp	.L6
.L10:
	movl	$0, %edi
	subq	$4, %rsp
	movl	%edi, -24(%rbp)
	movl	$0, %r9d
	movl	%r9d, -20(%rbp)
.L11:
	subq	$8, %rsp
.L12:
	movl	-20(%rbp), %r8d
	movl	-4(%rbp), %r9d
	cmpl	%r9d, %r8d
	jl	.L14
	jge	.L13
.L13:
	addq	$8, %rsp
	jmp	.L20
.L14:
	movl	$0, %r9d
	movl	%r9d, -24(%rbp)
.L15:
.L16:
	movl	-24(%rbp), %r8d
	movl	-8(%rbp), %r9d
	cmpl	%r9d, %r8d
	jl	.L18
	jge	.L17
.L17:
	addq	$0, %rsp
	jmp	.L19
.L18:
	subq	$4, %rsp
	movl	$0, -36(%rbp)
	movl	-20(%rbp), %r8d
	imull	$1, %r8d
	addl	-36(%rbp), %r8d
	movl	%r8d, -36(%rbp)
	subq	$4, %rsp
	movl	$0, -40(%rbp)
	movl	-24(%rbp), %r8d
	imull	$1, %r8d
	addl	-40(%rbp), %r8d
	movl	%r8d, -40(%rbp)
	movl	-36(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	a(%rip), %rbx
	movl	(%rdx, %rbx), %r8d
	movl	-40(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	b(%rip), %rbx
	movl	(%rdx, %rbx), %r9d
	imull	%r8d, %r9d
	subq	$4, %rsp
	movl	%r9d, -44(%rbp)
	movl	-44(%rbp), %edi
	subq	$4, %rsp
	movl	%edi, -48(%rbp)
	movl	-20(%rbp), %r8d
	movl	-24(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -52(%rbp)
	subq	$4, %rsp
	movl	$0, -56(%rbp)
	movl	-52(%rbp), %r8d
	imull	$1, %r8d
	addl	-56(%rbp), %r8d
	movl	%r8d, -56(%rbp)
	movl	-20(%rbp), %r8d
	movl	-24(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -60(%rbp)
	subq	$4, %rsp
	movl	$0, -64(%rbp)
	movl	-60(%rbp), %r8d
	imull	$1, %r8d
	addl	-64(%rbp), %r8d
	movl	%r8d, -64(%rbp)
	movl	-64(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	c(%rip), %rbx
	movl	(%rdx, %rbx), %r8d
	movl	-48(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -68(%rbp)
	movl	-68(%rbp), %r9d
	movl	-56(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	c(%rip), %rbx
	movl	%r9d, (%rdx, %rbx)
	movl	-24(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -72(%rbp)
	movl	-72(%rbp), %r9d
	movl	%r9d, -24(%rbp)
	addq	$40, %rsp
	jmp	.L15
.L19:
	movl	-20(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -36(%rbp)
	movl	-36(%rbp), %r9d
	movl	%r9d, -20(%rbp)
	addq	$12, %rsp
	jmp	.L11
.L20:
	movl	$0, %r9d
	movl	%r9d, -20(%rbp)
.L21:
	subq	$8, %rsp
	movl	-4(%rbp), %r8d
	movl	-8(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -36(%rbp)
	movl	-36(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -40(%rbp)
.L22:
	movl	-20(%rbp), %r8d
	movl	-40(%rbp), %r9d
	cmpl	%r9d, %r8d
	jl	.L24
	jge	.L23
.L23:
	addq	$16, %rsp
	jmp	.L25
.L24:
	subq	$4, %rsp
	movl	$0, -44(%rbp)
	movl	-20(%rbp), %r8d
	imull	$1, %r8d
	addl	-44(%rbp), %r8d
	movl	%r8d, -44(%rbp)
	subq	$4, %rsp
	movl	-44(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	c(%rip), %rbx
	movl	(%rdx, %rbx), %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-20(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -52(%rbp)
	movl	-52(%rbp), %r9d
	movl	%r9d, -20(%rbp)
	addq	$28, %rsp
	jmp	.L21
.L25:
	movl	$0, %eax
	addq	$24, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$24, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
