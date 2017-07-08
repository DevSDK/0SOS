     1                                  Add:
     2 00000000 55                      	push ebp
     3 00000001 89E5                    	mov ebp, esp
     4 00000003 8B4508                  	mov eax, dword[ebp + 8]
     5                                  	
     6 00000006 03450C                  	add eax, dword[ebp + 12]
     7 00000009 034510                  	add eax, dword[ebp + 16]
     8 0000000C 5D                      	pop ebp
     9                                  
    10 0000000D C20C00                  	ret 12
    11                                  
    12                                  main:
    13 00000010 55                      	push ebp
    14 00000011 89E5                    	mov ebp, esp
    15                                  
    16 00000013 83EC04                  	sub esp, 4
    17                                  	
    18 00000016 6A03                    	push 3
    19 00000018 6A02                    	push 2
    20 0000001A 6A01                    	push 1
    21 0000001C E8DFFFFFFF              	call Add
    22 00000021 8945FC                  	mov dword[ebp - 4], eax
    23 00000024 C3                      	ret
