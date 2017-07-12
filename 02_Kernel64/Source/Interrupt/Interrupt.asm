[BITS 64]

SECTION .text

global EnableInterrupt, DisableInterrupt, ReadFlags

EnableInterrupt:
    sti
    ret
DisableInterrupt:
    cli
    ret
ReadFlags:
    pushfq
    pop rax
    ret