[ORG 0x00]
[BITS 16]

SECTION .text
;이 코드는 부트로더에 의해 0x1000:0000 (0x10000)에 로드
START:
	mov ax, 0x1000
	mov ds, ax
	mov es, ax
	; DS, ES 세그먼트 지정
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; Enable A20 Gate

	mov ax, 0x2401  ;BIOS 서비스를 위한 코드. (0x2401 - A20 게이트 활성화)
	int 0x15		;BIOS 서비스 호출
	jc A20GATEENABLEERROR ; 만약 BISO 서비스로 A20게이트를 열 수 없다면 A20GATEENABLEERROR 레이블로 

	jmp A20GATEENABLESUCCESS

; 시스템 컨트롤 포트 0x92

; |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
; |    Hard Disk  |				  |Booting|  Not  |A20Gate| System|    
; |   LED Control |       		  |PW con.|  Use  |Control| Reset |


A20GATEENABLEERROR: ; 시스템 컨트롤 포트로 A20게이트 활성화 
	in al, 0x92		; 시스템 컨트롤 포트 0x92에서 1바이트를 읽어옴
	or al, 0x02     ; 두번째 비트 A20 게이트 제어를 1로 설정
	and al, 0xFE    ; 첫번째 비트 Reset 제외
	out 0x92, al    ; 다시 설정

A20GATEENABLESUCCESS:


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	cli		;Ignore Interrupt

	lgdt[GDTR]

	;	Now Switch Protected Mode.
	;
	;   CR0 Register
	;	Summary: 
	;		Disable Paging, Disable Cache, Internal FPU, Disable Align Check
	;
	;   fields:
	;
	;	|PG|CD|NW|  |  |  |  |  |  |  |  |  |  |AM|  |WP|
	;	31---------------------------------------------16
	;	| 0| 1| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0|
	;
	;	|  |  |  |  |  |  |  |  |  |  |NE|ET|TS|EM|MP|PE| 
	;	15----------------------------------------------0
	;	| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 1| 1| 1| 0| 1| 1|
	;
	;	PE(Protection Enable)	: 보호모드 진입 여부 설정
	;	MP(Monitor Coprocessor) : wait 또는 fwait 명령어 실행시 TS 필드 참고여부
	;	EM(Emulation)			: 프로세서 FPU 내장 여부 설정 
	;	ET(Extenstion Type)		: 1로 예약됨
	;	NE(Numeric Error)		: FPU 에러 처리를 내부 인터럽트(1), 외부 인터럽트 중 선택(0)
	;	WP(Write Protect)		: 쓰기 금지 기능 사용 어부[페이지 권한에 따른 접근 가능 여부]
	; 	AM(Alignment Mask)		: 주소정렬 기능 여부 설정
	;	NW(Not Write-Trough)	: 케시 정책 설정 (Write-trough(0), Wirte-Back(1))
	; 	CD(Cache Disable)		: 케시 비활성화
	; 	PG(Paging)				; 페이징 사용 여부
	;	0b01000000000000000000000000111011	
	; 	0x4000003B

	mov eax, 0x4000003B
	mov cr0, eax

	jmp dword 0x18 : (PROTECTEDMODE - $$ + 0x10000)	;32비트 영역으로 점프 
	


	; Following Code Now Prtoected Mode

[BITS 32]

PROTECTEDMODE:
	mov ax, 0x20 ; 각종 세그먼트 설렉터 설정
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax


	mov ss, ax
	mov esp, 0XFFFE	;스택 초기화
	mov ebp, 0XFFFE 
		
	push (SWITCHMESSAGE - $$ + 0x10000)
	push 0x0F
	push 4
	push 5
	call PRINT
	add  esp, 16


	push (SUCCESS - $$ + 0x10000)
	push 0x0A
	push 4
	push 60
	call PRINT
	add  esp, 16

	
	jmp dword 0x18:0x10200		;Let's Jump To C

PRINT:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ecx
	push edx

	mov eax, dword [ebp + 12]
	mov esi, 160
	mul esi
	mov edi, eax
		
	mov eax, dword[ebp + 8]
	mov esi, 2
	mul esi
	add edi, eax
	mov bl, byte[ebp + 16]
	mov esi, dword[ebp + 20]	
.PRINTLOOP:
	mov cl, byte[esi]
	
	cmp cl,0
	je PRINTEND
	
	mov byte[edi + 0xB8000], cl
	mov byte[edi + 0xB8000+1], bl	
	add esi,1
	add edi,2	
	jmp .PRINTLOOP
 
PRINTEND:
	pop edx
	pop ecx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; DATA AREA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

align 8, db 0

dw 0x0000	;For Align
;GDTR 
;크기[16]기준주소[32]

GDTR:
	dw GDTREND - GDT -1
	dd GDT - $$ + 0x10000

; GDT Entry 필드
; Limit or Size[32]: 세그먼트 크기
; Base Address [20]: 세그먼트 시작 주소
; Type		    [4]: 세그먼트 타입
; S 			[1]: 디스크립터 타입
; DPL			[2]: 권한
; P				[1]: 유효 여부
; AVL			[1]: 임시
; L				[1]: IA-32e 에서 64,32 구분
; D/B			[1]: 기본 연산 크기 (0-16, 1-32)
; G				[1]: 가중치 1일시 4KB 곱함

GDT:
	NULLDescriptor:
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x00
	db 0x00
	db 0x00
	IA_32eCodeDescriptor:
	dw 0xFFFF	; Limit[15:0]
	dw 0x0000	; Base[15:0]
	db 0x00		; Base[23:16] 
	db 0x9A		; P = 1, DPL = 0, Code Segment, Execute/Read
	db 0xAF		; G = 1, D = 0, L = 1, Limit [19:16]
	db 0x00		; Base[31:24]
	
	IA_32eDataDescriptor:
	dw 0xFFFF	; Limit[15:0]
	dw 0x0000	; Base[15:0]
	db 0x00		; Base[23:16] 
	db 0x92		; P = 1, DPL = 0, Code Segment, Execute/Read
	db 0xAF		; G = 1, D = 0, L = 1, Limit [19:16]
	db 0x00		; Base[31:24]

	;64비트 코드로 점프하기 위한 IA-32e 디스크립터

	CODEDescriptor:
	dw 0xFFFF	; Limit[15:0]
	dw 0x0000	; Base[15:0]
	db 0x00		; Base[23:16]
	db 0x9A		; P = 1, DPL = 0, Code Segment, Execute/Read
	db 0xCF		; G = 1, D = 1, L = 0, Limit [19:16]
	db 0x00		; Base[31:24]
	
	DATADescriptor:
	dw 0xFFFF	; Limit
	dw 0x0000	; Base
	db 0x00		; Base
	db 0x92		; P=1, DPL =0, Data Segment, Read/Write
	db 0xCF		; G= 1, D= 1, L= 0, Limit
	db 0x00		; Base
GDTREND:


SWITCHMESSAGE: db 'Switch to 32bit protected mode ....................... ',0
SUCCESS: db '[SUCCESS]',0


times 512 - ( $ - $$) db 0x00
