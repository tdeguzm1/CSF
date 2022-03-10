	.file	"c_hexmain.c"
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
	subq	$112, %rsp
	movl	$0, -4(%rbp)
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	hex_read@PLT
	movl	%eax, -8(%rbp)
	jmp	.L2
.L5:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	print_hex_offset@PLT
	addl	$1, -4(%rbp)
	movl	-8(%rbp), %edx
	leaq	-112(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	print_hex_equivalent@PLT
	movl	-8(%rbp), %edx
	leaq	-112(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	reprint_ascii_form@PLT
	cmpl	$15, -8(%rbp)
	jbe	.L7
	leaq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	hex_read@PLT
	movl	%eax, -8(%rbp)
.L2:
	cmpl	$0, -8(%rbp)
	jne	.L5
	jmp	.L4
.L7:
	nop
.L4:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
