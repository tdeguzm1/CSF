	.file	"c_hexmain.c"
	.text
	.section	.rodata
.LC0:
	.string	": "
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$240, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -232(%rbp)
	leaq	-224(%rbp), %rax
	movq	%rax, %rdi
	call	hex_read@PLT
	movl	%eax, -228(%rbp)
	jmp	.L2
.L5:
	movl	-232(%rbp), %eax
	sall	$4, %eax
	movl	%eax, %edx
	leaq	-112(%rbp), %rax
	movq	%rax, %rsi
	movl	%edx, %edi
	call	hex_format_offset@PLT
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	hex_write_string@PLT
	leaq	.LC0(%rip), %rdi
	call	hex_write_string@PLT
	addl	$1, -232(%rbp)
	movl	-228(%rbp), %edx
	leaq	-224(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	print_hex_equivalent@PLT
	movl	-228(%rbp), %edx
	leaq	-224(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	reprint_ascii_form@PLT
	cmpl	$15, -228(%rbp)
	jbe	.L8
	leaq	-224(%rbp), %rax
	movq	%rax, %rdi
	call	hex_read@PLT
	movl	%eax, -228(%rbp)
.L2:
	cmpl	$0, -228(%rbp)
	jne	.L5
	jmp	.L4
.L8:
	nop
.L4:
	movl	$0, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
