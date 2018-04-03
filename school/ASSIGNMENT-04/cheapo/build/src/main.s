	.file	"main.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Error: No value entered.\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"Error: Value must be greater than 2.\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB45:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	%edi, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	(%rsi), %rax
	movq	%rax, program_name(%rip)
	call	handle_options@PLT
	movslq	optind(%rip), %rax
	cmpl	%eax, %ebp
	je	.L7
	movq	(%rbx,%rax,8), %rdi
	movl	$1, %esi
	call	__xatoi@PLT
	cmpq	$2, %rax
	jle	.L8
	movzbl	%al, %edi
	call	init_tree@PLT
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	solve@PLT
	cmpb	$0, quiet(%rip)
	jne	.L4
	movq	%rbx, %rdi
	call	display@PLT
.L4:
	movq	%rbx, %rdi
	call	destroy_tree@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	movq	stderr(%rip), %rcx
	movl	$37, %edx
	movl	$1, %esi
	leaq	.LC1(%rip), %rdi
	call	fwrite@PLT
	movl	$6, %edi
	call	exit@PLT
.L7:
	movq	stderr(%rip), %rcx
	movl	$25, %edx
	movl	$1, %esi
	leaq	.LC0(%rip), %rdi
	call	fwrite@PLT
	movl	$5, %edi
	call	exit@PLT
	.cfi_endproc
.LFE45:
	.size	main, .-main
	.comm	LEAF_ONLY,1,1
	.comm	quiet,1,1
	.comm	program_name,8,8
	.ident	"GCC: (Gentoo 7.3.0-r1 p1.1) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
