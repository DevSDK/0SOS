[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x07C0:START

;;;;;;;;;;;;;;;;;
;;;; CONFIG ;;;;;
;;;;;;;;;;;;;;;;;



TOTALSECTOR: dw	0x02 
KERNEL32SECTOR: dw 0x02

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
	; 디스크 리셋
	mov ax, 0
	mov dl, 0
	int 0x13
	jc ERRORHANDLER
	;;;;;;;;;;; READ DISK SECTOR ;;;;;;;
	
	mov si, 0x1000
	mov es, si	; 읽을 메모리 세그먼트 어드레스 0x1000 지정	
	mov bx, 0x0000; 0x1000:0000 ( 0x10000 ) 로 로드할것임. 

	mov di, word[TOTALSECTOR] ; 현제 섹터 지정

.READ:
	cmp di, 0		
	je BREAKREAD	; 모든 섹터를 읽으면 빠져나감
	sub di, 0x1

	mov ah, 0x02
	mov al, 0x1			; 읽을 섹터 수(량)
	mov ch, byte[TRACK]	; 트랙
	mov cl, byte[SECTOR]; 섹터
	mov dh, byte[HEADER]; 해드
	mov dl, 0x00
	int 0x13			; int 13 인터럽트 (플로피 디스크)
	jc ERRORHANDLER

	;;;;;;;;;; calculate Address
	;플로피 디스크로부터 데이터를 읽어오는 코드
	; 
	add si, 0x0020	; 512 byte
	mov es, si
	mov al, byte[SECTOR]	
	add al, 0x01
	mov byte[SECTOR],al ;읽을 대상 섹터 1 증가 
	cmp al, 19			;19와 비교 (읽을 수 있는 섹터 1~18)
	jl .READ
	xor byte[HEADER], 0x01 ; 앞 해더 다 읽으면 뒤쪽 해더 읽음
	mov byte[SECTOR], 0x01 ; 섹터 처음부터
	cmp byte[HEADER], 0x00 ; 해더 초기화
	jne .READ
	add byte[TRACK], 0x01	; 트랙 증가
	jmp .READ
BREAKREAD:
	push LOADSUCCESS
	push 0xA
	push 60
	push 3
	call PRINT
	add sp,8
	; 다 읽었으면 읽은 위치 0x10000 으로 점프
	jmp 0x1000:0x0000

ERRORHANDLER:
	push DISKERROR
	push 0x0C
	push 60
	push 3
	call PRINT
	add sp, 8
	jmp $

PRINT: ;단순히 print 하는 함수임. 인자로는 x,y
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

SECTOR:	db 0x02 ; 부트로더를 제외한 섹터인 2부터 시작
HEADER:	db 0x00	
TRACK:	db 0x00






times 510 - ($ - $$) db 0x00

db 0x55
db 0xAA ;부트로더라는걸 알리기 위한 데이터. 
