	.file	"a.c"
	.text
	.p2align 4,,15
	.globl	Clear
	.type	Clear, @function
Clear:
.LFB0:
	.cfi_startproc
	movl	$753664, %eax
	.p2align 4,,10
	.p2align 3
.L3:
	movb	$0, (%rax)
	movb	$10, 1(%rax)
	addq	$2, %rax
	cmpq	$757664, %rax
	jne	.L3
	rep ret
	.cfi_endproc
.LFE0:
	.size	Clear, .-Clear
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
