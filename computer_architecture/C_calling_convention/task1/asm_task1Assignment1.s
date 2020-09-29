section	.rodata			; we define (global) read-only variables in .rodata section
	format_number: db "%d", 10, 0	; format number

section .bss			; we define (global) uninitialized variables in .bss section

section .text
	global assFunc
	extern printf
   extern c_checkValidity

assFunc:
	push ebp
	mov ebp, esp	
	pushad			

   mov ecx, dword [ebp+12]	; get function argument (y)
   mov ebx, dword [ebp+8]  ; get the 1 args (x)
   push dword ecx          ;
   push dword ebx          ;
   call c_checkValidity    ;

   cmp eax, 0           ;
   jnz  print_One       ;
   add ebx, ecx         ;
	push dword ebx			; call printf with 2 arguments -  
	push format_number	; 
	call printf    ;
	add esp, 16		; clean up stack after call
   jmp End        ;

print_One:
   sub ebx, ecx         ;
	push dword ebx			; call printf with 2 arguments -  
	push format_number	; 
	call printf    ;
	add esp, 16		; clean up stack after call  

End:
	popad			
	mov esp, ebp	
	pop ebp
	ret
