;os_api
	.global _syscall_printString 
	.global _syscall_readString
	.global _syscall_readSector
	.global _syscall_readFile
	.global _syscall_executeProgram
	.global _syscall_clearScreen
	.global _syscall_terminateProgram
	.global _syscall_writeSector
	.global _syscall_deleteFile
	.global _syscall_writeFile
	.global _syscall_copyFile
	.global _syscall_dir
	.global _syscall_createTXT
	.global _enableInterrupts
	.global _syscall_killProcess

;void enableInterrupts()
;call at the beginning of programs.  allows timer preemption
_enableInterrupts:
	sti
	ret

_syscall_killProcess:
	push bp
	mov bp, sp
	
	mov bx, [bp+4]
	mov ax, #0x9
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

_syscall_executeProgram:
	push bp
	mov bp, sp

	mov bx, [bp+4]
	mov ax, #0x4
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_terminateProgram:
	push bp
	mov bp, sp

	mov ax, #0x5
	int #0x21

	mov sp,bp
	pop bp
	ret	

_syscall_writeSector:
	push bp
	mov bp, sp

	mov cx, [bp+6]
	mov bx, [bp+4]
	mov ax, #0x6
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_deleteFile:
	push bp
	mov bp, sp
	
	mov bx, [bp+4]
	mov ax, #0x7
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_writeFile:
	push bp
	mov bp, sp

	mov dx, [bp+8]
	mov cx, [bp+6]
	mov bx, [bp+4]
	mov ax, #0x8
	int #0x21
	mov sp,bp
	pop bp
	ret

_syscall_dir:
	push bp
	mov bp, sp

	mov ax, #0xd
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

_syscall_createTXT:
	push bp
	mov bp, sp
	
	mov bx, [bp+4]
	mov ax, #0xC
	int #0x21

	mov sp,bp
	pop bp
	ret

_syscall_copyFile:
	push bp
	mov bp, sp

	mov cx, [bp+6]
	mov bx, [bp+4]
	mov ax, #0xb
	int #0x21
	mov sp,bp
	pop bp
	ret
