	.file	"c_hexfuncs.c"
	.text
	.globl	hex_read
	.type	hex_read, @function
hex_read:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$16, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	hex_read, .-hex_read
	.globl	hex_write_string
	.type	hex_write_string, @function
hex_write_string:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L4
.L5:
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	addq	$1, -8(%rbp)
.L4:
	movq	-24(%rbp), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L5
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	hex_write_string, .-hex_write_string
	.globl	to_hex
	.type	to_hex, @function
to_hex:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	cmpl	$9, -4(%rbp)
	ja	.L7
	movl	-4(%rbp), %eax
	addl	$48, %eax
	jmp	.L8
.L7:
	movl	-4(%rbp), %eax
	addl	$87, %eax
.L8:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	to_hex, .-to_hex
	.globl	hex_format_offset
	.type	hex_format_offset, @function
hex_format_offset:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$32, %rsp
	.cfi_offset 3, -24
	movl	%edi, -28(%rbp)
	movq	%rsi, -40(%rbp)
	movq	-40(%rbp), %rax
	addq	$8, %rax
	movb	$0, (%rax)
	movl	$7, -12(%rbp)
	jmp	.L10
.L11:
	movl	-28(%rbp), %eax
	andl	$15, %eax
	movl	-12(%rbp), %edx
	movslq	%edx, %rcx
	movq	-40(%rbp), %rdx
	leaq	(%rcx,%rdx), %rbx
	movl	%eax, %edi
	call	to_hex
	movb	%al, (%rbx)
	shrl	$4, -28(%rbp)
	subl	$1, -12(%rbp)
.L10:
	cmpl	$0, -12(%rbp)
	jns	.L11
	nop
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	hex_format_offset, .-hex_format_offset
	.globl	hex_format_byte_as_hex
	.type	hex_format_byte_as_hex, @function
hex_format_byte_as_hex:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset 3, -24
	movl	%edi, %eax
	movq	%rsi, -24(%rbp)
	movb	%al, -12(%rbp)
	movzbl	-12(%rbp), %eax
	shrb	$4, %al
	movzbl	%al, %eax
	movl	%eax, %edi
	call	to_hex
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movb	%dl, (%rax)
	movzbl	-12(%rbp), %eax
	andl	$15, %eax
	movq	-24(%rbp), %rdx
	leaq	1(%rdx), %rbx
	movl	%eax, %edi
	call	to_hex
	movb	%al, (%rbx)
	movq	-24(%rbp), %rax
	addq	$2, %rax
	movb	$0, (%rax)
	nop
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	hex_format_byte_as_hex, .-hex_format_byte_as_hex
	.globl	hex_to_printable
	.type	hex_to_printable, @function
hex_to_printable:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$31, -4(%rbp)
	jbe	.L14
	cmpb	$126, -4(%rbp)
	jbe	.L15
.L14:
	movl	$46, %eax
	jmp	.L16
.L15:
	movzbl	-4(%rbp), %eax
.L16:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	hex_to_printable, .-hex_to_printable
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
