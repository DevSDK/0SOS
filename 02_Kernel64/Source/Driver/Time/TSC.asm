[BITS 64]

SECTION .text

global ReadTSC

;타임 스탬프 카운터를 읽어서 반환
ReadTSC:
    push rdx
    rdtsc   ;타임 스탬프 카운터의 값을 rdx:rax에 저장
    shl rdx, 32 ; rdx 레지스터의 상위 32비트 tsc 값과 rax레지스터의
    or rax, rdx ; 하위 32비트 tsc값을 rax에 저장
    pop rdx
    ret