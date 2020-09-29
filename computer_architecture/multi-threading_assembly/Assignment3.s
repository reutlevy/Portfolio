
section .data
    calcFormat     : db "%s",0  
    stringFormat     : db "%s",10,0                         

    intFormat     : db "%d",10,0 
    floatFormat     : db "%lf",10,0 
    intFormatSscanf     : db "%d",0 
    floatFormatSscanf     : db "%f",0 

    hexFormat     : db "%X",0  
    newLine     : db "",10,0  
    stack_pointer_print     : db "stack_pointer=%d",10,0 
    error_args : db 'Error: Insufficient Number of Arguments on Stack', 10, 0 
    stack_overflow : db 'Error: Operand Stack Overflow', 10, 0
    hexFormatnew     : db "The result of the operation is = %X",10, 0 
    hexFormat1     : db "%X",10, 0 
 

    DFlag :dd 0            
    dataToNode :dd 0
    
    firstCharVal :dd 0
    secondCharVal :dd 0
    nodeVal :dd 0    

    bufferCounter :dd 0                 

    stack_size :dd 5    
    stack_pointer :dd 0

    calc db	'calc: ',0
    len1 equ $ - calc      

    curr_link :dd 0
    last_link :dd 0
    first_link :dd 0 
    buffer_size :dd 0 

    op_stack :dd 0
    pop_value :dd 0
    pop_value2 :dd 0
    pop_free1 : dd 0
    pop_free2 : dd 0


    sum_curry :dd 0
    
    dup_value :dd 0

    number_lentgh :dd 0

    num_of_args: dd 0

    counter : dd 0

    N : dd 0               
    R : dd 0
    K : dd 0
    d : dq 0.0
    seed : dd 0
    maxInt : dd 65535
    num360: dd 360
    num180: dd 180
    num120: dd 120
    num60: dd 60
    num100: dd 100
    num20: dd 20
    num10: dd 10
    num11: dd 1
    try  : dd 1.2
    pi dq    3.141592653589793238462


    msg     db      "sum = %f",10,0
    x	dq	0.0
    y	dq	0.0
    z	dd	0
    out100Random	dq	0
    speed	dq	2.0
    engal	dq	60.0

section .bss
    ; op_stack :    resb 1  
    buffer :    resb 80   
    buffer_length equ $ - buffer
    bufferToCopy :    resb 80   
    num1: resb 4 
        
   

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

%macro my_sscanf 3
    push ebp
    mov ebp, esp
    pushad
    mov ecx, %1 ; insert to
    mov ebx, %2 ; format
    mov edx, %3 ; string
    push ecx
    push ebx
    push edx
    call sscanf    
    add esp, 12
    popad
    mov esp, ebp
    pop ebp  
%endmacro


section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern sscanf   
  extern malloc 
  extern calloc 
  extern free 
  
  global _start
  extern stdin  
  extern stderr

  
main:
    
    push ebp
    mov ebp, esp 
    finit

    ; Args flag
    call check_Args
    call new_X        
    call new_Y        
    ; fld	qword [engal]	        ; need to convert 32-bit to 64-bit
	  ; fild	dword [num180]
	  ; fdiv
    ; fld    qword [pi] ;push single_value2 to fpu stack(st0)
    ; fmul
    ; fcos
    ; fstp	qword [out100Random]  
    ; my_print_float [out100Random+4], [out100Random], msg

    ; fild    dword [num120] ;push single_value2 to fpu stack(st0)
    ; fmul   
    ; fild    dword [num60] ;push single_value2 to fpu stack(st0)
    ; fsub
    ; fstp	qword [out100Random]  
    ; my_print_float [out100Random+4], [out100Random], msg
    
    

    
    

    ; finit
    ; fld dword [N]
    ; fstp qword [try]
    ; mov dword [num1], __float32__(3.25)
    ; my_print [num1], floatFormat 

    ;start stack
      ; call InitStack
      ; call menu

    mov esp, ebp
    pop ebp
    ret   

new_X:
		push ebp
    mov ebp, esp
    pushad    
    finit

    fld	qword [engal]	        ; need to convert 32-bit to 64-bit
	  fild	dword [num180]
	  fdiv
    fld    qword [pi] ;push single_value2 to fpu stack(st0)
    fmul
    fsin
    fld    qword [speed] 
    fmul
    fld    qword [x] 
    fadd        
    fstp	qword [x]  
    my_print_float [x+4], [x], msg    

    fld	qword [x]
    fild	dword [num100] 
    
    fcomi
    jc .sub100    
    jmp .endSub100
    
    .sub100:
    fstp	qword [x]  
    fld	qword [x]
    fsub
    fstp	qword [x]     

    .endSub100:

    my_print_float [x+4], [x], msg   
    
	  
    
    popad
    mov esp, ebp
    pop ebp 
		ret


new_Y:
		push ebp
    mov ebp, esp
    pushad    
    finit

    fld	qword [engal]	        ; need to convert 32-bit to 64-bit
	  fild	dword [num180]
	  fdiv
    fld    qword [pi] ;push single_value2 to fpu stack(st0)
    fmul
    fcos
    fld    qword [speed] 
    fmul
    fld    qword [y] 
    fadd        
    fstp	qword [y]  
    my_print_float [y+4], [y], msg   


    ; fild	dword [num180] 
    ; fstp	qword [y]  
    ; my_print_float [y+4], [y], msg   

    fld	qword [y]
    fild	dword [num100] 
    
    fcomi
    jc .sub100    
    jmp .endSub100
    
    .sub100:
    fstp	qword [y]  
    fld	qword [y]
    fsub
    fstp	qword [y]     

    .endSub100:

    my_print_float [y+4], [y], msg   
    
	  
    
    popad
    mov esp, ebp
    pop ebp 
		ret



next_random_60:
		push ebp
    mov ebp, esp
    pushad    
    call next_Random    
    finit

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num120] ;push single_value2 to fpu stack(st0)
    fmul   
    fild    dword [num60] ;push single_value2 to fpu stack(st0)
    fsub
    fstp	qword [out100Random]  
    my_print_float [out100Random+4], [out100Random], msg
    
	  
    
    popad
    mov esp, ebp
    pop ebp 
		ret


next_random_10:
		push ebp
    mov ebp, esp
    pushad
    call next_Random    
    finit

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num20] ;push single_value2 to fpu stack(st0)
    fmul   
    fild    dword [num10] ;push single_value2 to fpu stack(st0)
    fsub
    fstp	qword [out100Random]  
    my_print_float [out100Random+4], [out100Random], msg
    
	  
    
    popad
    mov esp, ebp
    pop ebp 
		ret


next_XOrY:
		push ebp
    mov ebp, esp
    pushad
    call next_Random    
    finit

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num100] ;push single_value2 to fpu stack(st0)
    fmul   
    fstp	qword [out100Random]  
    my_print_float [out100Random+4], [out100Random], msg   
    
	  
    
    popad
    mov esp, ebp
    pop ebp 
		ret





next_Random:
		push ebp
    mov ebp, esp
    pushad
    mov eax, 0
    mov eax , dword [seed]
    mov edx, 16
    
    xor esi,esi 
     
	start_Random_loop:   
    xor ebx, ebx
    mov ecx,eax ;16 bit        
    and ax,0x0001   
    mov bx,ax    
        
    mov eax,ecx ;14 bit
    and ax,0x0004
    shr ax, 2	
		xor bx,ax    
    
    mov eax,ecx ;13 bit
    and ax,0x0008
    shr ax, 3    
		xor bx,ax
     
    
    xor eax,eax
    mov eax,ecx ;11 bit
    and ax,0x0020
    shr ax,5         
    xor bx,ax
    
    xor bx,si   

		mov eax,ecx  
    
    
    xor esi, esi
    mov esi,eax ;16 bit    
    and si,0x0001      
        
    ror bx,1
    shr ax,1
    or ax,bx  
    

    dec edx
    cmp edx,0
		jne start_Random_loop

    mov [seed],eax
    popad
    mov esp, ebp
    pop ebp 
		ret



check_Args:

    ;start get args
    mov eax, dword[ebp +8] ; argc
    mov ebx, dword[ebp +12] ; argv
    mov dword[num_of_args], eax
    mov ecx, 0

    loopArgs:

    cmp ecx, 0x00
    je endLoopArgs

    xor edx, edx
    xor eax, eax

    mov eax, dword[ebx]
    
    cmp ecx, 0x01
    je NArg
    cmp ecx, 0x02
    je RArg
    cmp ecx, 0x03
    je KArg
    cmp ecx, 0x04
    je DArg
    cmp ecx, 0x05
    je seedArg

    NArg:
    my_sscanf N, intFormatSscanf, eax  
    ; my_print [N], intFormat 
    jmp endLoopArgs

    RArg:
    my_sscanf R, intFormatSscanf, eax  
    ; my_print [R], intFormat 
    jmp endLoopArgs

    KArg:
    my_sscanf K, intFormatSscanf, eax  
    ; my_print [K], intFormat 
    jmp endLoopArgs

    DArg:
    my_print eax, stringFormat 
    my_sscanf d, floatFormatSscanf, eax  
    ; my_print [d], floatFormat 
    jmp endLoopArgs

    seedArg:
    my_sscanf seed, intFormatSscanf, eax 
    ; my_print [seed], intFormat 
    jmp endLoopArgs    
    

    endLoopArgs:
     
    inc ecx 
    add ebx, 4 ; get next arg
    cmp ecx, dword[num_of_args] ; comp index of args to args number
    jne loopArgs ; jamp if we didnt get to the end

    ret



  



