 section .data
    stringFormat    : db "%s",10,0
    floatf    : db "%f",10,0
	message         : db "scheduler is here", 0
	message1         : db "co-routines are running", 0
   message2         : db "target is destroyed!!!!", 0
    counter          : dd 0
 
 section .text
    global startCo
    global initCo
    global targetfunction
    global do_resume
    extern printf
    extern schedule
    extern CORS
    extern get_my_loc
    extern Xtarget
    extern Ytarget
    extern Speedtarget
    extern Angletarget
    extern resume
    extern next_XOrY
    extern out100Random
    extern endCo
    extern x
    extern y
    extern new_X
    extern new_Y
    extern next_speed
    extern endfunctioncors
    extern speed
    global targetfunction
    extern engal
    extern COi
    extern next_angle

%macro my_print 2
    push ebp
    mov ebp, esp
    pushad
    mov ecx, %1
    mov edx, %2
    push ecx;
    push edx
    call printf    
    add esp, 8
    popad
    mov esp, ebp
    pop ebp  
%endmacro

   %macro my_print_float 3
    push ebp
    mov ebp, esp
    pushad
    ; push	dword [temp+4] 		; push temp as 2 32-bit words
	  ; push	dword [temp]
    ; push    dword msg		; address of format string
    ; call    printf			; Call C function
    ; add     esp, 12			; pop stack 3*4 bytes
    mov ecx, %1    
    mov edx, %2
    mov ebx, %3
    push ecx;
    push edx
    push ebx
    call printf    
    add esp, 12
    popad
    mov esp, ebp
    pop ebp  
   %endmacro

targetfunction:
       ;---- get x new value----;

        call new_X
        fld   qword [x] 
        fstp   qword [Xtarget]

     
    ;---- get y new value----; 

        call new_Y
        fld   qword [y] 
        fstp   qword [Ytarget]

      ;---- get speed new value----; 
        fld   qword [Speedtarget]   
        fstp   qword [speed]
        call next_speed
        fld   qword [speed] 
        fstp   qword [Speedtarget]

     ;---- get angle new value----; 
        fld   qword [Angletarget]   
        fstp   qword [engal]
        call next_angle
        fld   qword [engal] 
        fstp   qword [Angletarget]

endprinterf:
  mov EBX, COi
  call resume
  jmp targetfunction