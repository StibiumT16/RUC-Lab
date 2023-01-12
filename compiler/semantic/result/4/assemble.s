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
	subq	$144, %rsp
	movl	$1, %edi
	subq	$4, %rsp
	movl	%edi, -148(%rbp)
	movl	$1, %edi
	subq	$4, %rsp
	movl	%edi, -152(%rbp)
.L1:
	subq	$8, %rsp
.L2:
	movl	-148(%rbp), %r8d
	movl	$5, %r9d
	cmpl	%r9d, %r8d
	jl	.L3
	jge	.L4
.L3:
	movl	-152(%rbp), %r8d
	movl	$5, %r9d
	cmpl	%r9d, %r8d
	jl	.L5
	jge	.L4
.L4:
	addq	$8, %rsp
	jmp	.L6
.L5:
	subq	$4, %rsp
	movl	$0, -164(%rbp)
	movl	-152(%rbp), %r8d
	imull	$1, %r8d
	addl	-164(%rbp), %r8d
	movl	%r8d, -164(%rbp)
	movl	-148(%rbp), %r8d
	imull	$6, %r8d
	addl	-164(%rbp), %r8d
	movl	%r8d, -164(%rbp)
	movl	-148(%rbp), %r8d
	movl	-152(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -168(%rbp)
	movl	-168(%rbp), %r9d
	movl	-164(%rbp), %ebx
	cltq
	movl	%r9d, -144(%rbp, %rbx, 4)
	movl	-152(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -172(%rbp)
	movl	-172(%rbp), %r9d
	movl	%r9d, -152(%rbp)
	addq	$20, %rsp
	jmp	.L1
.L6:
.L7:
	subq	$8, %rsp
.L8:
	movl	-148(%rbp), %r8d
	movl	$5, %r9d
	cmpl	%r9d, %r8d
	jl	.L9
	jge	.L10
.L9:
	movl	-152(%rbp), %r8d
	movl	$6, %r9d
	cmpl	%r9d, %r8d
	jl	.L11
	jge	.L10
.L10:
	addq	$8, %rsp
	jmp	.L12
.L11:
	subq	$4, %rsp
	movl	$0, -164(%rbp)
	movl	-152(%rbp), %r8d
	imull	$1, %r8d
	addl	-164(%rbp), %r8d
	movl	%r8d, -164(%rbp)
	movl	-148(%rbp), %r8d
	imull	$6, %r8d
	addl	-164(%rbp), %r8d
	movl	%r8d, -164(%rbp)
	movl	-148(%rbp), %r8d
	movl	-152(%rbp), %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -168(%rbp)
	movl	-168(%rbp), %r9d
	movl	-164(%rbp), %ebx
	cltq
	movl	%r9d, -144(%rbp, %rbx, 4)
	movl	-148(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -172(%rbp)
	movl	-172(%rbp), %r9d
	movl	%r9d, -148(%rbp)
	addq	$20, %rsp
	jmp	.L7
.L12:
	subq	$4, %rsp
	movl	$0, -156(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-156(%rbp), %r8d
	movl	%r8d, -156(%rbp)
	movl	$1, %r8d
	imull	$6, %r8d
	addl	-156(%rbp), %r8d
	movl	%r8d, -156(%rbp)
	subq	$4, %rsp
	movl	$0, -160(%rbp)
	movl	$4, %r8d
	imull	$1, %r8d
	addl	-160(%rbp), %r8d
	movl	%r8d, -160(%rbp)
	movl	$1, %r8d
	imull	$6, %r8d
	addl	-160(%rbp), %r8d
	movl	%r8d, -160(%rbp)
	movl	-156(%rbp), %ebx
	cltq
	movl	-144(%rbp, %rbx, 4), %r8d
	movl	-160(%rbp), %ebx
	cltq
	movl	-144(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -164(%rbp)
	subq	$4, %rsp
	movl	$0, -168(%rbp)
	movl	$5, %r8d
	imull	$1, %r8d
	addl	-168(%rbp), %r8d
	movl	%r8d, -168(%rbp)
	movl	$4, %r8d
	imull	$6, %r8d
	addl	-168(%rbp), %r8d
	movl	%r8d, -168(%rbp)
	movl	-164(%rbp), %r8d
	movl	-168(%rbp), %ebx
	cltq
	movl	-144(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -172(%rbp)
	subq	$4, %rsp
	movl	-172(%rbp), %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$176, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
