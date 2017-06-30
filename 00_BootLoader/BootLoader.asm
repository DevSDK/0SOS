[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x07C0:START

;;;;;;;;;;;;;;;;;
;;;; CONFIG ;;;;;
;;;;;;;;;;;;;;;;;

TOTALSECTOR: dw 2



START:
	mov ax, 0x07C0
	mov ds, ax


	;;;;;;;;;; STACK INITALIZATION ;;;;;;;;;;
	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xFFFE
	mov bp, 0XFFFE

	mov si, 0

	call CLEAR 


	;;;;;;;;;; PRINT HELLO ;;;;;;;

	push HELLO	
	push 0x0B
	push 30
	push 1
	call PRINT
	add sp, 8
	;;;;;;;;;; PRINT LOAD INFO ;;;;;;
	push LOADINFO
	push 0x0F
	push 5
	push 3
	call PRINT
	add sp, 8


	
DISKRESET:
	mov ax, 0
	mov dl, 0
	int 0x13
	jc ERRORHANDLER

	;;;;;;;;;;; READ DISK SECTOR ;;;;;;;
	mov si, 0x1000
	mov es, si
	mov bx, 0x0000

	mov di, word[TOTALSECTOR]

.READ:
	cmp di, 0
	je BREAKREAD
	sub di, 0x1

	mov ah, 0x02
	mov al, 0x1
	mov ch, byte[TRACK]
	mov cl, byte[SECTOR]
	mov dh, byte[HEADER]
	mov dl, 0x00
	int 0x13
	jc ERRORHANDLER

	;;;;;;;;;; calculate adress
	add si, 0x0020

	mov es, si

	mov al, byte[SECTOR]
	add al, 0x01
	mov byte[SECTOR],al
	cmp al, 19
	jl .READ

	xor byte[HEADER], 0x01
	mov byte[SECTOR], 0x01

	cmp byte[HEADER], 0x00
	jne .READ
	add byte[TRACK], 0x01	
	jmp .READ
BREAKREAD:
	push LOADSUCCESS
	push 0xA
	push 60
	push 3
	call PRINT
	add sp,8

	jmp 0x1000:0x0000

ERRORHANDLER:
	push DISKERROR
	push 0x0C
	push 60
	push 3
	call PRINT
	add sp, 8
	jmp $

PRINT:
	push bp,
	mov bp, sp
	
	push es
	push si
	push di
	push ax
	push cx
	push dx	

	mov ax, 0xB800
	mov es, ax


	mov ax, word[bp + 4]
	mov si, 160
	mul si
	mov di, ax

	mov ax, word[bp + 6]
	
	mov si, 2
	mul si
	add di, ax

	mov si, word[bp+10]
	mov bl, byte[bp+8]
	
.PRINTLOOP
	mov cl, byte[si]
		
	cmp cl,0
	je .ENDPRINTLOOP

	mov byte[es:di], cl
	mov byte[es:di+1], bl
	add si, 1
	add di, 2
	jmp .PRINTLOOP

.ENDPRINTLOOP
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret


CLEAR:
	push bp
	mov bp, sp
	
	push es
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800
	mov es, ax
	
.CLEARLOOP:
	mov byte[es:si],0
	mov byte[es:si+1], 0x0F
	add si,2
	
	cmp si,80*25*2
	jl .CLEARLOOP
	
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret	



HELLO: db 'Welcome 0SOS',0
DISKERROR: db '[ERROR]', 0
LOADSUCCESS: db '[SUCCESS]',0
LOADINFO: db 'Now Loading From DISK ................................', 0

SECTOR:	db 0x02
HEADER:	db 0x00
TRACK:	db 0x00






times 510 - ($ - $$) db 0x00

db 0x55
db 0xAA
