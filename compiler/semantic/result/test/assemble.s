.LC0:
	.string	"%d"
.LC1:
	.string	"%d\n"
	.section	.rodata
	.align	4
	.type	N, @object
	.size	N, 4
N:
	.long	100
	.text
	.globl	a
	.data
	.align	32
	.type	a, @object
	.size	a, 400
a:
	.zero	400
	.text
	.globl	func
	.type	main, @function
func:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$4, %rsp
	movl	32(%rbp), %r8d
	movl	%r8d, -4(%rbp)
.L1:
	movl	-4(%rbp), %r8d
	movl	$1, %r9d
	cmpl	%r9d, %r8d
	je	.L3
	jne	.L2
.L2:
	movl	-4(%rbp), %r8d
	movl	$0, %r9d
	cmpl	%r9d, %r8d
	je	.L3
	jne	.L4
.L3:
	subq	$12, %rsp
	movl	$1, %eax
	addq	$16, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$12, %rsp
	jmp	.L5
.L4:
	subq	$12, %rsp
	movl	-4(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -20(%rbp)
	subq	$12, %rsp
	movl	-20(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -36(%rbp)
	call	func
	subq	$4, %rsp
	movl	%eax, -40(%rbp)
	movl	-4(%rbp), %r8d
	movl	$2, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -44(%rbp)
	movl	-44(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -48(%rbp)
	call	func
	subq	$4, %rsp
	movl	%eax, -52(%rbp)
	movl	-40(%rbp), %r8d
	movl	-52(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -56(%rbp)
	movl	-56(%rbp), %eax
	addq	$56, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$52, %rsp
.L5:
	addq	$4, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	movl	$10, %edi
	subq	$4, %rsp
	movl	%edi, -4(%rbp)
	subq	$4, %rsp
	movl	$0, -8(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-8(%rbp), %r8d
	movl	%r8d, -8(%rbp)
	movl	-4(%rbp), %r8d
	movl	$2, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -12(%rbp)
	movl	-12(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -16(%rbp)
	call	func
	subq	$4, %rsp
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %r9d
	movl	-8(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	a(%rip), %rbx
	movl	%r9d, (%rdx, %rbx)
	subq	$4, %rsp
	movl	$0, -24(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-24(%rbp), %r8d
	movl	%r8d, -24(%rbp)
	subq	$8, %rsp
	movl	-24(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	a(%rip), %rbx
	movl	(%rdx, %rbx), %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$32, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
