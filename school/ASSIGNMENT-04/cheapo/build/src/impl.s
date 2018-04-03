	.file	"impl.c"
	.text
	.p2align 4,,15
	.globl	init_tree
	.type	init_tree, @function
init_tree:
.LFB58:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	%edi, %ebp
	movl	$32, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	xmalloc@PLT
	movl	$1, %edi
	movq	%rax, %rbx
	call	xmalloc@PLT
	xorl	%edi, %edi
	movq	%rax, (%rbx)
	movb	%bpl, (%rax)
	movl	$1, %eax
	movw	%ax, 8(%rbx)
	call	xmalloc@PLT
	xorl	%edx, %edx
	movb	$1, 26(%rbx)
	movq	%rax, 16(%rbx)
	movq	%rbx, %rax
	movw	%dx, 24(%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE58:
	.size	init_tree, .-init_tree
	.p2align 4,,15
	.globl	destroy_tree
	.type	destroy_tree, @function
destroy_tree:
.LFB59:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$152, %rsp
	.cfi_def_cfa_offset 208
	cmpb	$0, 24(%rdi)
	movq	%rdi, 120(%rsp)
	je	.L5
	movq	$0, 136(%rsp)
	movl	$0, 112(%rsp)
.L22:
	movq	120(%rsp), %rax
	movq	136(%rsp), %rcx
	movq	16(%rax), %rax
	movq	(%rax,%rcx), %rax
	cmpb	$0, 24(%rax)
	movq	%rax, 104(%rsp)
	je	.L6
	movq	$0, 128(%rsp)
	movl	$0, 116(%rsp)
.L21:
	movq	104(%rsp), %rax
	movq	128(%rsp), %rcx
	movq	16(%rax), %rax
	movq	(%rax,%rcx), %rax
	cmpb	$0, 24(%rax)
	movq	%rax, 88(%rsp)
	je	.L7
	movq	$0, 96(%rsp)
	movl	$0, 84(%rsp)
.L20:
	movq	88(%rsp), %rax
	movq	96(%rsp), %rsi
	movq	16(%rax), %rax
	movq	(%rax,%rsi), %rax
	cmpb	$0, 24(%rax)
	movq	%rax, 64(%rsp)
	je	.L8
	movq	$0, 72(%rsp)
	movl	$0, 80(%rsp)
.L19:
	movq	64(%rsp), %rax
	movq	72(%rsp), %rsi
	movq	16(%rax), %rax
	movq	(%rax,%rsi), %rax
	cmpb	$0, 24(%rax)
	movq	%rax, 48(%rsp)
	je	.L9
	movq	$0, 56(%rsp)
	movl	$0, 44(%rsp)
.L18:
	movq	48(%rsp), %rax
	movq	56(%rsp), %rsi
	movq	16(%rax), %rax
	movq	(%rax,%rsi), %rax
	cmpb	$0, 24(%rax)
	movq	%rax, 24(%rsp)
	je	.L10
	movq	$0, 32(%rsp)
	movl	$0, 40(%rsp)
.L17:
	movq	24(%rsp), %rax
	movq	32(%rsp), %rcx
	movq	16(%rax), %rax
	movq	(%rax,%rcx), %rax
	cmpb	$0, 24(%rax)
	movq	%rax, 8(%rsp)
	je	.L11
	movq	$0, 16(%rsp)
	movl	$0, 4(%rsp)
.L16:
	movq	8(%rsp), %rax
	movq	16(%rsp), %rcx
	movq	16(%rax), %rax
	movq	(%rax,%rcx), %rbx
	cmpb	$0, 24(%rbx)
	je	.L12
	xorl	%r12d, %r12d
	xorl	%ebp, %ebp
.L15:
	movq	16(%rbx), %rax
	movq	(%rax,%r12), %r13
	cmpb	$0, 24(%r13)
	je	.L13
	xorl	%r15d, %r15d
	xorl	%r14d, %r14d
.L14:
	movq	16(%r13), %rax
	addl	$1, %r14d
	movq	(%rax,%r15), %rdi
	addq	$8, %r15
	call	destroy_tree
	movzbl	24(%r13), %eax
	cmpl	%eax, %r14d
	jl	.L14
.L13:
	movq	16(%r13), %rdi
	addl	$1, %ebp
	addq	$8, %r12
	call	free@PLT
	movq	0(%r13), %rdi
	call	free@PLT
	movq	%r13, %rdi
	call	free@PLT
	movzbl	24(%rbx), %eax
	cmpl	%eax, %ebp
	jl	.L15
.L12:
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	8(%rsp), %rax
	addl	$1, 4(%rsp)
	movl	4(%rsp), %edx
	addq	$8, 16(%rsp)
	movzbl	24(%rax), %eax
	cmpl	%eax, %edx
	jl	.L16
.L11:
	movq	8(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	24(%rsp), %rax
	addl	$1, 40(%rsp)
	movl	40(%rsp), %esi
	addq	$8, 32(%rsp)
	movzbl	24(%rax), %eax
	cmpl	%eax, %esi
	jl	.L17
.L10:
	movq	24(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	48(%rsp), %rax
	addl	$1, 44(%rsp)
	movl	44(%rsp), %ecx
	addq	$8, 56(%rsp)
	movzbl	24(%rax), %eax
	cmpl	%eax, %ecx
	jl	.L18
.L9:
	movq	48(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	64(%rsp), %rax
	addl	$1, 80(%rsp)
	movl	80(%rsp), %esi
	addq	$8, 72(%rsp)
	movzbl	24(%rax), %eax
	cmpl	%eax, %esi
	jl	.L19
.L8:
	movq	64(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	88(%rsp), %rax
	addl	$1, 84(%rsp)
	movl	84(%rsp), %ecx
	addq	$8, 96(%rsp)
	movzbl	24(%rax), %eax
	cmpl	%eax, %ecx
	jl	.L20
.L7:
	movq	88(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	104(%rsp), %rax
	addl	$1, 116(%rsp)
	addq	$8, 128(%rsp)
	movl	116(%rsp), %esi
	movzbl	24(%rax), %eax
	cmpl	%eax, %esi
	jl	.L21
.L6:
	movq	104(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	movq	%rbx, %rdi
	call	free@PLT
	movq	120(%rsp), %rax
	addl	$1, 112(%rsp)
	addq	$8, 136(%rsp)
	movl	112(%rsp), %esi
	movzbl	24(%rax), %eax
	cmpl	%esi, %eax
	jg	.L22
.L5:
	movq	120(%rsp), %rbx
	movq	16(%rbx), %rdi
	call	free@PLT
	movq	(%rbx), %rdi
	call	free@PLT
	addq	$152, %rsp
	.cfi_def_cfa_offset 56
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	free@PLT
	.cfi_endproc
.LFE59:
	.size	destroy_tree, .-destroy_tree
	.p2align 4,,15
	.globl	new_node
	.type	new_node, @function
new_node:
.LFB60:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rsi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movq	%rdi, %rbp
	movl	$32, %edi
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	call	xmalloc@PLT
	cmpb	$0, 26(%rbp)
	sete	26(%rax)
	xorl	%edx, %edx
	xorl	%edi, %edi
	movq	%rax, %rbx
	movw	%dx, 24(%rax)
	movq	%r13, (%rax)
	movq	%r12, 8(%rax)
	call	xmalloc@PLT
	movq	%rax, 16(%rbx)
	movzbl	24(%rbp), %edx
	cmpb	25(%rbp), %dl
	je	.L34
	movq	16(%rbp), %rax
.L35:
	leal	1(%rdx), %ecx
	movb	%cl, 24(%rbp)
	movq	%rbx, (%rax,%rdx,8)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L34:
	.cfi_restore_state
	addl	$1, %edx
	movq	16(%rbp), %rdi
	movb	%dl, 25(%rbp)
	movzbl	%dl, %edx
	leaq	0(,%rdx,8), %rsi
	call	xrealloc@PLT
	movzbl	24(%rbp), %edx
	movq	%rax, 16(%rbp)
	jmp	.L35
	.cfi_endproc
.LFE60:
	.size	new_node, .-new_node
	.p2align 4,,15
	.globl	state_cpy
	.type	state_cpy, @function
state_cpy:
.LFB61:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movzwl	8(%rdi), %ebp
	movq	%rbp, %rdi
	call	xmalloc@PLT
	movq	(%r12), %rsi
	movq	%rbp, %rdx
	movq	%rax, %rdi
	call	memcpy@PLT
	xorl	%edx, %edx
	popq	%rbx
	.cfi_def_cfa_offset 24
	movw	%bp, %dx
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE61:
	.size	state_cpy, .-state_cpy
	.p2align 4,,15
	.globl	state_append
	.type	state_append, @function
state_append:
.LFB62:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rsi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movl	%edx, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movzwl	8(%rdi), %ebp
	movq	(%rdi), %rdi
	addl	%edx, %ebp
	movzwl	%bp, %esi
	call	xrealloc@PLT
	movzwl	8(%rbx), %edi
	movzwl	%r12w, %edx
	movq	%r13, %rsi
	movq	%rax, (%rbx)
	addq	%rax, %rdi
	call	memcpy@PLT
	movw	%bp, 8(%rbx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE62:
	.size	state_append, .-state_append
	.comm	LEAF_ONLY,1,1
	.comm	quiet,1,1
	.comm	program_name,8,8
	.ident	"GCC: (Gentoo 7.3.0-r1 p1.1) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
