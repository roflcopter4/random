	.file	"display.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"\\-["
.LC1:
	.string	"+ ["
.LC2:
	.string	"%d, "
.LC3:
	.string	"%d]"
.LC4:
	.string	" \033[1m\033[32mMAX\033[0m"
.LC5:
	.string	" \033[1m\033[31mMIN\033[0m"
	.text
	.p2align 4,,15
	.type	do_display, @function
do_display:
.LFB59:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	.cfi_offset 15, -24
	movl	%ecx, %r15d
	pushq	%r14
	pushq	%r13
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	movq	%rsi, %r13
	pushq	%r12
	pushq	%rbx
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, %rbx
	movq	%r13, %rdi
	subq	$56, %rsp
	movq	stdout(%rip), %rsi
	movq	%rdx, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -56(%rbp)
	xorl	%eax, %eax
	call	fputs@PLT
	movl	$3, %edx
	movl	$1, %esi
	movq	stdout(%rip), %rcx
	testb	%r15b, %r15b
	je	.L2
	leaq	.LC0(%rip), %rdi
	call	fwrite@PLT
	movzwl	8(%rbx), %eax
	testl	%eax, %eax
	je	.L11
.L3:
	xorl	%r14d, %r14d
	leaq	.LC3(%rip), %r12
	.p2align 4,,10
	.p2align 3
.L7:
	movq	(%rbx), %rdx
	subl	$1, %eax
	leaq	.LC2(%rip), %rsi
	movzbl	(%rdx,%r14), %edx
	cmpl	%r14d, %eax
	jg	.L28
	movq	%r12, %rsi
.L28:
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movzwl	8(%rbx), %eax
	leal	1(%r14), %edx
	addq	$1, %r14
	cmpl	%edx, %eax
	jg	.L7
	testb	%r15b, %r15b
	je	.L6
.L11:
	cmpb	$0, LEAF_ONLY(%rip)
	jne	.L4
.L5:
	cmpb	$0, 26(%rbx)
	jne	.L30
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
.L14:
	movq	%r13, %rdi
	call	strlen@PLT
	movl	$8224, %ecx
	addq	%r13, %rax
	movw	%cx, (%rax)
	movb	$0, 2(%rax)
.L15:
	movzbl	24(%rbx), %r15d
	testl	%r15d, %r15d
	je	.L1
	movq	-72(%rbp), %rax
	xorl	%r12d, %r12d
	leaq	2(%rax), %rcx
	addq	$17, %rax
	andq	$-16, %rax
	movq	%rcx, -80(%rbp)
	movq	%rax, -88(%rbp)
.L17:
	movq	%rsp, -72(%rbp)
	subq	-88(%rbp), %rsp
	movq	%r13, %rsi
	leal	1(%r12), %r14d
	movq	%rsp, %rdi
	call	strcpy@PLT
	movq	16(%rbx), %rdx
	xorl	%ecx, %ecx
	cmpl	%r14d, %r15d
	sete	%cl
	movq	%rsp, %rsi
	movq	(%rdx,%r12,8), %rdi
	movq	-80(%rbp), %rdx
	addq	$1, %r12
	call	do_display
	movzbl	24(%rbx), %r15d
	movq	-72(%rbp), %rsp
	cmpl	%r14d, %r15d
	jg	.L17
.L1:
	movq	-56(%rbp), %rax
	xorq	%fs:40, %rax
	jne	.L31
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	.cfi_restore_state
	leaq	.LC1(%rip), %rdi
	call	fwrite@PLT
	movzwl	8(%rbx), %eax
	testl	%eax, %eax
	jne	.L3
	.p2align 4,,10
	.p2align 3
.L6:
	movq	stdout(%rip), %rsi
	movl	$10, %edi
	call	_IO_putc@PLT
	movq	%r13, %rdi
	call	strlen@PLT
	movl	$8316, %edx
	addq	%r13, %rax
	movw	%dx, (%rax)
	movb	$0, 2(%rax)
	jmp	.L15
	.p2align 4,,10
	.p2align 3
.L4:
	cmpb	$0, 24(%rbx)
	je	.L5
	movq	stdout(%rip), %rsi
	movl	$10, %edi
	call	_IO_putc@PLT
	jmp	.L14
.L30:
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	jmp	.L14
.L31:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE59:
	.size	do_display, .-do_display
	.section	.rodata.str1.1
.LC6:
	.string	"  "
	.text
	.p2align 4,,15
	.globl	display
	.type	display, @function
display:
.LFB58:
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
	leaq	.LC3(%rip), %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	xorl	%ebx, %ebx
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	movq	stdout(%rip), %rsi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	20(%rsp), %r12
	movl	$32, 20(%rsp)
	movq	%r12, %rdi
	call	fputs@PLT
	movl	$3, %edx
	movq	stdout(%rip), %rcx
	movl	$1, %esi
	leaq	.LC0(%rip), %rdi
	call	fwrite@PLT
	movzwl	8(%rbp), %eax
	testl	%eax, %eax
	je	.L34
	.p2align 4,,10
	.p2align 3
.L33:
	movq	0(%rbp), %rdx
	subl	$1, %eax
	leaq	.LC2(%rip), %rsi
	movzbl	(%rdx,%rbx), %edx
	cmpl	%ebx, %eax
	jg	.L52
	movq	%r13, %rsi
.L52:
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movzwl	8(%rbp), %eax
	leal	1(%rbx), %edx
	addq	$1, %rbx
	cmpl	%edx, %eax
	jg	.L33
.L34:
	cmpb	$0, LEAF_ONLY(%rip)
	je	.L37
	cmpb	$0, 24(%rbp)
	jne	.L38
.L37:
	cmpb	$0, 26(%rbp)
	jne	.L53
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
.L40:
	movl	$4, %edx
	leaq	.LC6(%rip), %rsi
	movq	%r12, %rdi
	call	__strcat_chk@PLT
	movzbl	24(%rbp), %r13d
	testl	%r13d, %r13d
	je	.L32
	xorl	%r14d, %r14d
	xorl	%ebx, %ebx
	leaq	14(%rsp), %r15
.L42:
	movl	$6, %edx
	movq	%r12, %rsi
	movq	%r15, %rdi
	addl	$1, %ebx
	call	__strcpy_chk@PLT
	movq	16(%rbp), %rax
	xorl	%ecx, %ecx
	cmpl	%r13d, %ebx
	sete	%cl
	movl	$6, %edx
	movq	%r15, %rsi
	movq	(%rax,%r14), %rdi
	addq	$8, %r14
	call	do_display
	movzbl	24(%rbp), %r13d
	cmpl	%r13d, %ebx
	jl	.L42
.L32:
	movq	24(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L54
	addq	$40, %rsp
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
.L53:
	.cfi_restore_state
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	jmp	.L40
	.p2align 4,,10
	.p2align 3
.L38:
	movq	stdout(%rip), %rsi
	movl	$10, %edi
	call	_IO_putc@PLT
	jmp	.L40
.L54:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE58:
	.size	display, .-display
	.comm	LEAF_ONLY,1,1
	.comm	quiet,1,1
	.comm	program_name,8,8
	.ident	"GCC: (Gentoo 7.3.0-r1 p1.1) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
