.global _printChar 
.global _readChar
.global _readSector
_printChar:
	push bp
	mov bp, sp
	
	mov al,[bp+4]
	mov ah, #0x0e
	int #0x10

	pop bp
	ret

_readChar:
	mov ah, #0x0
	int #0x16
	mov ah, #0x0
	ret

_readSector:
	push bp
	mov bp, sp
	sub sp, 6
	push ax
	push bx
	push cx

	mov ax, [bp+6]
	mov bx, #0x24
	cdq
	idiv bx
	mov [bp-2], ax

	mov ax, [bp+6]
	mov bx, #0x12
	cdq
	idiv bx
	add dx, #0x1
	mov [bp-4], dx

	mov eax, #0x0
	mov ax, [bp+6]
	mov bx, #0x12
	cdq
	idiv bx	
	mov bx, #0x2
	cdq
	idiv bx
	mov [bp-6], dx

	mov ah, #0x2
	mov al, #0x1
	mov bx, [bp+4]
	mov ch, [bp-2]
	mov cl, [bp-4]
	mov dh, [bp-6]
	mov dl, #0x0
	int #0x13

	pop cx
	pop bx
	pop ax
	mov sp, bp
	pop bp
	ret