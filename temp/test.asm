Add:
	push ebp
	mov ebp, esp
	mov eax, dword[ebp + 8]
	
	add eax, dword[ebp + 12]
	add eax, dword[ebp + 16]
	pop ebp

	ret 12

main:
	push ebp
	mov ebp, esp

	sub esp, 4
	
	push 3
	push 2
	push 1
	call Add
	mov dword[ebp - 4], eax
	ret
