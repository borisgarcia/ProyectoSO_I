	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"Error"
	.text
	.globl	handleInterrupt21
	.type	handleInterrupt21, @function
handleInterrupt21:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L2
	movl	-8(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	printString
	jmp	.L6
.L2:
	cmpl	$1, -4(%rbp)
	jne	.L4
	movl	-8(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	readString
	jmp	.L6
.L4:
	cmpl	$2, -4(%rbp)
	jne	.L5
	movl	-12(%rbp), %edx
	movl	-8(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	movl	$0, %eax
	call	readSector
	jmp	.L6
.L5:
	cmpl	$3, -4(%rbp)
	jne	.L6
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printString
.L6:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	handleInterrupt21, .-handleInterrupt21
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
