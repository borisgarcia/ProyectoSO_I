;os_api
	.global _syscall_printString 
	.global _syscall_readString
	.global _syscall_readSector
	.global _syscall_readFile
	.global _syscall_executeProgram
	.global _syscall_clearScreen
	.global _syscall_terminateProgram

_syscall_terminateProgram:
	push bp
	mov bp, sp

	mov ax, #0x5
	int #0x21

	mov sp,bp
	pop bp
	ret	

_syscall_clearScreen:
	push bp
	mov bp, sp

	mov ax, #0xa
	int #0x21

	mov sp,bp
	pop bp
	ret	


_syscall_executeProgram:
	push bp
	mov bp, sp

	mov cx, [bp+6]
	mov bx, [bp+4]
	mov ax, #0x4
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_readFile:
	push bp
	mov bp, sp

	mov cx, [bp+6]
	mov bx, [bp+4]
	mov ax, #0x3
	int #0x21
	mov sp,bp
	pop bp
	ret

_syscall_printString:
	push bp
	mov bp, sp
	
	mov bx, [bp+4]
	mov ax, #0x0
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_readString:
	push bp
	mov bp, sp

	mov bx, [bp+4]
	mov ax, #0x1
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_readSector:
	push bp
	mov bp, sp

	mov cx, [bp+6]
	mov bx, [bp+4]
	mov ax, #0x2
	int #0x21

	mov sp,bp
	pop bp
	ret
