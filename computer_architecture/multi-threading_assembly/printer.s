   global startCo
    global initCo
    global do_resume
    extern printf
    extern CORS
    extern N
    extern endCo
    extern schedule
    global schedulerfunction
    global printerrfunction
    extern get_my_loc
    extern Xtarget
    extern Ytarget
    extern resume

    section .data
        stringFormat    : db "%s",10,0
        message         : db "scheduler is here", 0
        message1        : db "We are on printer function !!!!", 0
        fmt             :	DB "[%d] = %d", 10, 0
        intFormat       : db "%d",10,0 
        targetprinter   : db "the current target coordinates are x= %d, y= %d",10,0 
        targetX        : db "X value of the target is = %f ,",0 
        targetY        : db "Y value of the target is = %f", 10, 0
        floatFormat     : db "%f",10,0 
        intFormatSscanf     : db "%d",0 
        floatFormatSscanf     : db "%f",0 
        DroneNumber     : db "Drone number %d ", 0
        droneX          : db "coordinates are x= %f ", 0
        droneY          : db ",y= %f ", 0
        droneangle       : db "angle = %f ", 0
        dronespeed       : db "speed = %f ", 0
        dronedestroyed   : db "number Of Destroyed Targets = %d ", 10, 0
        messageend      : db "Wnd print", 10 , 0
        Drownprinter    : db "Drone number %d coordinates are x= %d, y= %d angle= %d speed= %d number Of Destroyed Targets = %d ",10,0 
        SPMAIN          : dd 0
        CURR            : dd 0
        SPP	            : dd 4     ; offset of pointer to co-routine stack in co-routine struct 
        CODEP	        : dd 0	     ; offset of pointer to co-routine function in co-routine struct 
        SPT	            : dd 0     ; offset of pointer to co-routine stack in co-routine struct 
        counter         : dd 0

    section .text
 

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

    %macro my_print_target 3
        push ebp
        mov ebp, esp
        pushad
        mov ecx, %1
        mov edx, %2
        mov ebx, %3
        push ecx;
        push edx
        push edx
        call printf    
        add esp, 12
        popad
        mov esp, ebp
        pop ebp  
    %endmacro

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


printerrfunction:
 ;my_print dword[counter], intFormat
 my_print_float [Xtarget+4], [Xtarget], targetX
 my_print_float [Ytarget+4], [Ytarget], targetY
 mov dword[counter], 0

    loopprinterf:
        mov edx, dword[counter]
        cmp edx , dword[N] ; N is the number of co-routines
        je endprinterf
        
        call get_my_loc
        imul edx, 40 ; index * 20
        lea edx, [eax + edx]
        
        my_print dword[counter], DroneNumber
        my_print_float [edx+4], [edx], droneX
        my_print_float [edx+12], [edx+8], droneY
        my_print_float [edx+20], [edx+16], dronespeed
        my_print_float [edx+28], [edx+24], droneangle
        my_print dword[edx+36], dronedestroyed
        inc dword[counter]
        jmp loopprinterf

endprinterf:
xor EBX, EBX
xor EDX, EDX
mov EBX, schedule
call resume
jmp printerrfunction
ret


