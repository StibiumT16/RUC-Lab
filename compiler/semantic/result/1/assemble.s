.LC0:
	.string	"%d"
.LC1:
	.string	"%d\n"
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
	movl	-4(%rbp), %r8d
	movl	-8(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -20(%rbp)
	movl	-20(%rbp), %edi
	subq	$4, %rsp
	movl	%edi, -24(%rbp)
	subq	$8, %rsp
	movl	-24(%rbp), %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$32, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
