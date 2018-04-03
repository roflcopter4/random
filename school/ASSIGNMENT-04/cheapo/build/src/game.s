	.file	"game.c"
	.text
	.p2align 4,,15
	.type	split, @function
split:
.LFB62:
	.cfi_startproc
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	movl	$9, %r8d
	pushq	-8(%r10)
	pushq	%rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	movq	%rsp, %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x58,0x6
	.cfi_escape 0x10,0xf,0x2,0x76,0x78
	.cfi_escape 0x10,0xe,0x2,0x76,0x70
	.cfi_escape 0x10,0xd,0x2,0x76,0x68
	.cfi_escape 0x10,0xc,0x2,0x76,0x60
	pushq	%rbx
	subq	$3136, %rsp
	.cfi_escape 0x10,0x3,0x2,0x76,0x50
	movq	%fs:40, %rax
	movq	%rax, -56(%rbp)
	xorl	%eax, %eax
	movzbl	%dil, %eax
	movzbl	%dil, %edi
	leaq	18(,%rax,4), %rax
	andl	$2032, %eax
	subq	%rax, %rsp
	movl	$2, %eax
	shrx	%rax, %rsp, %rdx
	movq	%rdx, %rax
	leaq	0(,%rdx,4), %r9
	negq	%rax
	andl	$7, %eax
	leal	7(%rax), %ecx
	cmpl	$9, %ecx
	cmovb	%r8d, %ecx
	leal	-1(%rdi), %r8d
	cmpl	%r8d, %ecx
	ja	.L2
	testl	%eax, %eax
	je	.L18
	movl	$1, 0(,%rdx,4)
	cmpl	$1, %eax
	je	.L19
	movl	$2, 4(,%rdx,4)
	cmpl	$2, %eax
	je	.L20
	movl	$3, 8(,%rdx,4)
	cmpl	$3, %eax
	je	.L21
	movl	$4, 12(,%rdx,4)
	cmpl	$4, %eax
	je	.L22
	movl	$5, 16(,%rdx,4)
	cmpl	$5, %eax
	je	.L23
	movl	$6, 20(,%rdx,4)
	cmpl	$7, %eax
	jne	.L24
	movl	$7, 24(,%rdx,4)
	movl	$7, %ecx
.L3:
	movl	%edi, %r10d
	vmovd	%ecx, %xmm0
	leaq	(%r9,%rax,4), %rdx
	vmovdqa	.LC1(%rip), %ymm3
	subl	%eax, %r10d
	vpbroadcastd	%xmm0, %ymm0
	vmovdqa	.LC2(%rip), %ymm2
	xorl	%eax, %eax
	movl	%r10d, %r8d
	vpaddd	.LC0(%rip), %ymm0, %ymm0
	shrl	$3, %r8d
	.p2align 4,,10
	.p2align 3
.L5:
	vpaddd	%ymm2, %ymm0, %ymm1
	addl	$1, %eax
	vpaddd	%ymm3, %ymm0, %ymm0
	addq	$32, %rdx
	vmovdqa	%ymm1, -32(%rdx)
	cmpl	%eax, %r8d
	ja	.L5
	movl	%r10d, %eax
	andl	$-8, %eax
	addl	%eax, %ecx
	cmpl	%eax, %r10d
	je	.L34
	leal	1(%rcx), %eax
	movslq	%ecx, %rcx
	movl	%eax, (%r9,%rcx,4)
	cmpl	%edi, %eax
	jge	.L34
	vzeroupper
.L15:
	leal	1(%rax), %edx
	movslq	%eax, %rcx
	movl	%edx, (%r9,%rcx,4)
	cmpl	%edx, %edi
	jle	.L6
	leal	2(%rax), %ecx
	movslq	%edx, %rdx
	movl	%ecx, (%r9,%rdx,4)
	cmpl	%ecx, %edi
	jle	.L6
	leal	3(%rax), %edx
	movslq	%ecx, %rcx
	movl	%edx, (%r9,%rcx,4)
	cmpl	%edx, %edi
	jle	.L6
	leal	4(%rax), %ecx
	movslq	%edx, %rdx
	movl	%ecx, (%r9,%rdx,4)
	cmpl	%ecx, %edi
	jle	.L6
	leal	5(%rax), %edx
	movslq	%ecx, %rcx
	movl	%edx, (%r9,%rcx,4)
	cmpl	%edx, %edi
	jle	.L6
	leal	6(%rax), %ecx
	movslq	%edx, %rdx
	movl	%ecx, (%r9,%rdx,4)
	cmpl	%ecx, %edi
	jle	.L6
	leal	7(%rax), %edx
	movslq	%ecx, %rcx
	movl	%edx, (%r9,%rcx,4)
	cmpl	%edx, %edi
	jle	.L6
	leal	8(%rax), %ecx
	movslq	%edx, %rdx
	movl	%ecx, (%r9,%rdx,4)
	cmpl	%ecx, %edi
	jle	.L6
	leal	9(%rax), %edx
	movslq	%ecx, %rcx
	movl	%edx, (%r9,%rcx,4)
	cmpl	%edx, %edi
	jle	.L6
	leal	10(%rax), %ecx
	movslq	%edx, %rdx
	movl	%ecx, (%r9,%rdx,4)
	cmpl	%ecx, %edi
	jle	.L6
	leal	11(%rax), %edx
	movslq	%ecx, %rcx
	movl	%edx, (%r9,%rcx,4)
	cmpl	%edx, %edi
	jle	.L6
	leal	12(%rax), %ecx
	movslq	%edx, %rdx
	movl	%ecx, (%r9,%rdx,4)
	cmpl	%ecx, %edi
	jle	.L6
	movslq	%ecx, %rcx
	addl	$13, %eax
	movl	%eax, (%r9,%rcx,4)
	.p2align 4,,10
	.p2align 3
.L6:
	xorl	%r10d, %r10d
	.p2align 4,,10
	.p2align 3
.L11:
	movl	(%r9,%r10,4), %r8d
	movl	%r10d, %ebx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L10:
	movl	(%r9,%rax,4), %edx
	leal	(%r8,%rdx), %ecx
	cmpl	%edi, %ecx
	je	.L38
	addq	$1, %rax
	cmpl	%eax, %edi
	jg	.L10
	xorl	%eax, %eax
.L9:
	leal	1(%r10), %ebx
	movb	%al, -1088(%rbp,%r10)
	addq	$1, %r10
	leaq	-1088(%rbp), %r15
	cmpl	%r10d, %edi
	jg	.L11
	movl	%ebx, (%rsi)
	movslq	%ebx, %rdi
	salq	$4, %rdi
	call	malloc@PLT
	movq	%rax, -3160(%rbp)
	movq	%rax, %r13
.L16:
	leal	-1(%rbx), %eax
	xorl	%ebx, %ebx
	addq	$1, %rax
	movq	%rax, -3144(%rbp)
	leaq	-3136(%rbp), %rax
	movq	%rax, -3152(%rbp)
	.p2align 4,,10
	.p2align 3
.L14:
	movzbl	(%r15,%rbx), %r14d
	addq	$16, %r13
	movq	%r14, %rdi
	call	malloc@PLT
	movq	%r14, %rdx
	movq	%rax, %r9
	movq	-3152(%rbp), %rax
	movq	%r9, %rdi
	leaq	(%rax,%rbx,2), %rsi
	addq	$1, %rbx
	call	memcpy@PLT
	movw	%r14w, -8(%r13)
	movq	%rax, -16(%r13)
	cmpq	-3144(%rbp), %rbx
	jne	.L14
	movq	-3160(%rbp), %rax
.L1:
	movq	-56(%rbp), %rsi
	xorq	%fs:40, %rsi
	jne	.L39
	leaq	-48(%rbp), %rsp
	popq	%rbx
	popq	%r10
	.cfi_remember_state
	.cfi_def_cfa 10, 0
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L38:
	.cfi_restore_state
	cmpl	%edx, %r8d
	jge	.L8
	movb	%r8b, -3136(%rbp,%r10,2)
	movl	$2, %eax
	movb	%dl, -3135(%rbp,%r10,2)
	jmp	.L9
.L8:
	movl	%ebx, (%rsi)
	movslq	%ebx, %rdi
	salq	$4, %rdi
	call	malloc@PLT
	movq	%rax, -3160(%rbp)
	testl	%ebx, %ebx
	je	.L1
	leaq	-1088(%rbp), %r15
	movq	%rax, %r13
	jmp	.L16
.L34:
	vzeroupper
	jmp	.L6
.L20:
	movl	$2, %ecx
	jmp	.L3
.L19:
	movl	$1, %ecx
	jmp	.L3
.L18:
	xorl	%ecx, %ecx
	jmp	.L3
.L2:
	movl	$1, 0(,%rdx,4)
	movl	$1, %eax
	jmp	.L15
.L21:
	movl	$3, %ecx
	jmp	.L3
.L22:
	movl	$4, %ecx
	jmp	.L3
.L23:
	movl	$5, %ecx
	jmp	.L3
.L24:
	movl	$6, %ecx
	jmp	.L3
.L39:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE62:
	.size	split, .-split
	.p2align 4,,15
	.type	do_solve, @function
do_solve:
.LFB61:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rdi, %r15
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
	subq	$72, %rsp
	.cfi_def_cfa_offset 128
	movzwl	8(%rdi), %ecx
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	testw	%cx, %cx
	je	.L41
	leaq	28(%rsp), %rax
	xorl	%ebx, %ebx
	leaq	32(%rsp), %r12
	movq	%rax, (%rsp)
	leaq	tid(%rip), %r13
.L47:
	movq	(%r15), %rax
	movzbl	(%rax,%rbx), %edi
	cmpb	$2, %dil
	ja	.L69
.L42:
	leal	1(%rbx), %eax
	movzwl	%cx, %edx
	addq	$1, %rbx
	cmpl	%eax, %edx
	jg	.L47
.L41:
	cmpq	%r15, ROOT(%rip)
	je	.L70
.L40:
	movq	56(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L71
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
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
	ret
	.p2align 4,,10
	.p2align 3
.L69:
	.cfi_restore_state
	movq	(%rsp), %rsi
	call	split
	movl	28(%rsp), %edx
	movq	%rax, 8(%rsp)
	testl	%edx, %edx
	jle	.L43
	movq	%rax, %r14
	xorl	%ebp, %ebp
.L44:
	movq	%r15, %rdi
	call	state_cpy@PLT
	movq	%r12, %rdi
	movq	%rax, 32(%rsp)
	movq	%rdx, 40(%rsp)
	movq	(%r14), %rax
	movzbl	(%rax), %edx
	movq	32(%rsp), %rax
	movb	%dl, (%rax,%rbx)
	movzwl	8(%r14), %eax
	leal	-1(%rax), %edx
	movq	(%r14), %rax
	movzwl	%dx, %edx
	leaq	1(%rax), %rsi
	call	state_append@PLT
	movzwl	40(%rsp), %esi
	movq	32(%rsp), %rdi
	call	quick_sort@PLT
	movq	32(%rsp), %rsi
	movq	40(%rsp), %rdx
	movq	%r15, %rdi
	call	new_node@PLT
	cmpq	%r15, ROOT(%rip)
	jne	.L45
	cmpl	$11, %ebp
	jg	.L45
	leaq	0(%r13,%rbp,8), %rdi
	movq	%rax, %rcx
	leaq	wrapper(%rip), %rdx
	xorl	%esi, %esi
	call	pthread_create@PLT
.L68:
	movq	(%r14), %rdi
	addq	$16, %r14
	call	free@PLT
	leal	1(%rbp), %eax
	addq	$1, %rbp
	cmpl	%eax, 28(%rsp)
	jg	.L44
.L43:
	movq	8(%rsp), %rdi
	call	free@PLT
	movzwl	8(%r15), %ecx
	jmp	.L42
	.p2align 4,,10
	.p2align 3
.L45:
	movq	%rax, %rdi
	call	do_solve
	jmp	.L68
	.p2align 4,,10
	.p2align 3
.L70:
	movl	28(%rsp), %eax
	testl	%eax, %eax
	jle	.L40
	movl	$1, %ebx
	leaq	-8+tid(%rip), %rbp
	jmp	.L49
	.p2align 4,,10
	.p2align 3
.L72:
	cmpl	$11, %eax
	jg	.L40
.L49:
	movq	0(%rbp,%rbx,8), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movl	%ebx, %eax
	addq	$1, %rbx
	cmpl	%eax, 28(%rsp)
	jg	.L72
	jmp	.L40
.L71:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE61:
	.size	do_solve, .-do_solve
	.p2align 4,,15
	.type	wrapper, @function
wrapper:
.LFB60:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	do_solve
	xorl	%edi, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE60:
	.size	wrapper, .-wrapper
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"Solving for %d tokens.\n"
	.text
	.p2align 4,,15
	.globl	solve
	.type	solve, @function
solve:
.LFB59:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	(%rdi), %rax
	movq	%rdi, %rbx
	leaq	.LC3(%rip), %rsi
	movl	$1, %edi
	movzbl	(%rax), %edx
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movq	%rbx, ROOT(%rip)
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	do_solve
	.cfi_endproc
.LFE59:
	.size	solve, .-solve
	.comm	tid,96,32
	.comm	ROOT,8,8
	.comm	LEAF_ONLY,1,1
	.comm	quiet,1,1
	.comm	program_name,8,8
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.align 32
.LC1:
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.long	8
	.align 32
.LC2:
	.long	1
	.long	1
	.long	1
	.long	1
	.long	1
	.long	1
	.long	1
	.long	1
	.ident	"GCC: (Gentoo 7.3.0-r1 p1.1) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
