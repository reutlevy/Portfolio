  extern schedulerfunction
  extern printf
  extern fprintf 
  extern sscanf   
  extern malloc 
  extern calloc 
  extern free 
  global _start
  extern stdin  
  extern stderr
  extern do_resume
  global target
  global Fields
 ; global endCo
  extern targetfunction
  global N
  global CORS
  global speed_last
  global function_cors
  extern printerrfunction
  global get_my_loc
  global K
  global printer
  global Xtarget
  global Ytarget
  extern resume
  global schedule
  global next_XOrY
  global out100Random
  extern function_cors
  global COi
  global x
  global y
  global new_X
  global Speedtarget
  global Angletarget
  global new_Y
  global numberdrones
  global speed
  global engal
  global next_angle
  global next_speed
  global d
  global endofthegame
  global R
  extern checkaliminate

section .bss
    ; op_stack :    resb 1  
    buffer :    resb 80   
    buffer_length equ $ - buffer
    bufferToCopy :    resb 80   
    CURR:	resd	1
    SPT:	resd	1   ; temporary stack pointer
    SPMAIN:	resd	1   ; stack pointer of main
    STKSZ	equ	16*1024	     ; co-routine stack size
    STKDrone	resb STKSZ    ; co-routine stack size
    STKprinter	resb STKSZ    ; co-routine stack size
    STKtarget	resb STKSZ    ; co-routine stack size
    STscheduel resb STKSZ 
    myArray: resb DARRAY_size
    CORS   : resd 1
        
section .data
    stringFormat     : db "%s",10,0                         
    intFormat     : db "%d",10,0 
    winnerfrm     : db "The winner is %d",10,0 
    floatFormat     : db "%f",10,0 
    floatFormat1     : db "The first angle is %f",10,0 
    intFormatSscanf     : db "%d",0 
    floatFormatSscanf     : db "%e",0 
    FMT1: db "Function1, co %d", 10, 0 
    msg     db      "sum = %f",10,0
    message         : db "function in process", 0
    message1         : db "return to main", 10,0
    fmt             :	DB "[%d] = %d", 10, 0
    winner        : db 0
    hexFormat     : db "%X",0  
    newLine     : db "",10,0
    maxtargets   : db 0  


    Fields      : dd 0
    OtherCORS       : dd 0
    numberdrones    : dd 0
    DFlag :dd 0            
    dataToNode       :dd 0
    Xvalue          : dd 5
    Yvalue          : dd 6
    Dvalue          : dd 0
    SpeedValue      : dd 30
    Xtarget          : dq 0
    maxInt : dd 65535
    Ytarget          : dq 0
    Speedtarget       : dq 0
    Angletarget       : dd 0
    num120: dd 120
    num60: dd 60
    num601: dq 60.0
    num100: dd 100
    num0: dq 0.0
    num360: dd 360
    num20: dd 20
    num10: dd 10
    try  : dd 1.2
    SPP	            : dd 4     ; offset of pointer to co-routine stack in co-routine struct 
	  CODEP	        : dd 0	     ; offset of pointer to co-routine function in co-routine struct 
    destroyedTarget: dd 0
    out100Random	dq	0
    x	dq	0.0
    y	dq	0.0
    speed	dq	13.0
    engal	dq	60.0
    pi dq    3.141592653589793238462
    num180: dd 180

    op_stack :dd 0
    pop_value :dd 0
    pop_value2 :dd 0
    pop_free1 : dd 0
    pop_free2 : dd 0
    speed_last: dq 0
    COi:	   dd  function_cors 		; pointer to co-routine function
	           dd  STKSZ+STKDrone	 	      ; pointer to the beginning of co-routine stack
    target:	   dd  targetfunction 		; pointer to co-routine function
	           dd  STKSZ+STKtarget	 	      ; pointer to the beginning of co-routine stack
    schedule:  dd  schedulerfunction 		; pointer to co-routine function
	           dd  STKSZ+STscheduel	 	      ; pointer to the beginning of co-routine stack
    printer:  dd  printerrfunction 		; pointer to co-routine function
	            dd  STKSZ+STKprinter	 	      ; pointer to the beginning of co-routine stack



    dup_value :dd 0

    number_lentgh :dd 0

    num_of_args: dd 0

    counter : dd 0

    N : dd 0               
    R : dd 0
    K : dd 0
    d : dq 0.0
    D : dd 0
    seed : dd 0



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

%macro safe_malloc 1
    push ebp
    mov ebp, esp    
    push %1;
    push 1
    call calloc; 
    add esp, 4        
    mov esp, ebp
    pop ebp
  ;  add word[stack_pointer], 5; -- the stack pointer noe changed
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


section .text
  align 16
  global main

  struc DARRAY
	.ptr: resd 1	; Pointer to our block of memory. (contents of the array)
	.count: resd 1	; Number of elements currently allocated.
	endstruc


main:
    
    push ebp
    mov ebp, esp 

    call check_Args   

    call InitCorsField

;--------- init Target Fields----;
initTfields:
  xor eax, eax

  call next_XOrY
  fld   qword [out100Random]   
  fstp	qword [Xtarget]

  call next_XOrY
  fld   qword [out100Random]   
  fstp	qword [Ytarget]

 ;call getRandomAngle
  mov ecx, dword[Dvalue]
  mov [Angletarget], ecx

  call next_XOrY
  fld   qword [out100Random]   
  fstp	qword [Speedtarget]

;-------- init CoRs-----;
  initcornew:
  xor ebx,ebx
  mov ebx, dword COi        ; get pointer to COi struct
  mov eax, dword[ebx]        ; get initial EIP value – pointer to COi function
  mov [SPT], ESP              ; save ESP value
  mov esp, [ebx+4]          ; get initial ESP value – pointer to COi stack
  push eax                    ; push initial “return” address
  pushfd                      ; push flags
  pushad                      ; push all other registers
  mov [ebx+4], esp          ; save new SPi value (after all the pushes)
  mov ESP, [SPT] 
 
 ;------ init target-----;
  inittarget:
  xor ebx,ebx
  mov ebx, dword target        ; get pointer to COi struct
  mov eax, dword[ebx]        ; get initial EIP value – pointer to COi function
  mov [SPT], ESP              ; save ESP value
  mov esp, [ebx+4]          ; get initial ESP value – pointer to COi stack
  push eax                    ; push initial “return” address
  pushfd                      ; push flags
  pushad                      ; push all other registers
  mov [ebx+4], esp          ; save new SPi value (after all the pushes)
  mov ESP, [SPT] 

 ;----- init schedule-----; 
   initschedule: 
   xor ebx,ebx
   xor eax, eax
   mov ebx, dword schedule      ; get pointer to COi struct
   mov eax, dword[ebx]        ; get initial EIP value – pointer to COi function
   mov [SPT], esp              ; save ESP value
   mov esp, [ebx+4]          ; get initial ESP value – pointer to COi stack
   push eax                    ; push initial “return” address
   pushfd                      ; push flags
   pushad                      ; push all other registers
   mov [ebx+4], esp          ; save new SPi value (after all the pushes)
   mov ESP, [SPT] 

;------ init printerco-----;
    initprinter: 
  xor ebx,ebx
  mov ebx, dword printer		; get co-routine ID number
	mov eax, [ebx]	; get pointer to COi struct
;	mov eax, [ebx+4]            ; get initial EIP value – pointer to COi function
	mov [SPT], ESP	               ; save ESP value
	mov esp, [EBX+4]                  ; get initial ESP value – pointer to COi stack
	push eax 	                                         ; push initial “return” address
	pushfd		                  ; push flags
	pushad		               ; push all other registers
	mov [ebx+4], esp              ; save new SPi value (after all the pushes)
	mov ESP, [SPT]  
    
  startCo:
	mov [SPMAIN], ESP		; save ESP of main ()
	mov EBX, schedule		; gets ID of a scheduler co-routine
	jmp do_resume	

  endofthegame:
  call check_winner
    
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
  ;  my_print [N], intFormat 
    mov edx, dword[N]
    mov dword[numberdrones], edx
    jmp endLoopArgs

    RArg:
    my_sscanf R, intFormatSscanf, eax  
  ;  my_print [R], intFormat 
    jmp endLoopArgs

    KArg:
    my_sscanf K, intFormatSscanf, eax  
    jmp endLoopArgs

    DArg:
    my_sscanf D, intFormat, eax
    fild	dword [D] 
    fstp	qword [d] 
  ;  fld dword[ebx]          ; 
  ;  fstp qword [d]
    jmp endLoopArgs

    seedArg:
    my_sscanf seed, intFormatSscanf, eax 
  ;  my_print [seed], intFormat 
    jmp endLoopArgs 
    

    endLoopArgs:
     
    inc ecx 
    add ebx, 4 ; get next arg
    cmp ecx, dword[num_of_args] ; comp index of args to args number
    jne loopArgs ; jamp if we didnt get to the end

    ret


 
;-------------Init fields for each Core----------;
InitCorsField:
  push ebp
  mov ebp, esp
  pushad 
  mov edx, 0
  xor eax, eax
  xor ecx, ecx
  xor edx, edx

  mov eax, dword[N]
	mov dword [myArray + DARRAY.count], eax
  mov dword[counter], 0

  call next_random_60
  fld   qword [out100Random]
  fstp  qword [speed]   

  mov ecx, dword[N]
  imul ecx, 40              ; we have 4 fields for each drone- x,y,angle and speed. each one is 4 bytes
  safe_malloc ecx             ; 4 for each co-routine
  mov dword [myArray + DARRAY.ptr], eax 

  loopFields:
  mov edx, dword[counter]
 ; my_print dword[counter], intFormat
  cmp edx , dword[N] ; N is the number of co-routines
  je endFields
  
  mov eax, dword [myArray + DARRAY.ptr]
  mov edx, dword [counter]
  imul edx, 40 ; 0- X 8-Y 16-speed 24- Angle 32-Target 36- Alive 
	lea edx, [eax + edx]

  ;----- get x value----;
  call next_XOrY
  fld   qword [out100Random]   
  fstp	qword [edx]

  ;----- get y value----;
  call next_XOrY
  fld   qword [out100Random]   
  fstp	qword [edx+8]

  ;----- get speed value----;
  call next_XOrY
  fld   qword [out100Random]   
  fstp	qword [edx+16]

  ;----- get angle value----;
  call next_random_360
  fld   qword [out100Random]  
  fstp	qword [edx+24]
 ; my_print_float [edx+28], [edx+24], msg

;----- alive ?-----;
  mov ecx, 1
  mov [edx+32], ecx

;----- number destroyed-----;
  mov ecx, 0
  mov [edx+36], ecx
    
  inc dword[counter]
  jmp loopFields


  endFields:
  popad
  mov esp, ebp
  pop ebp
  ret


get_my_loc:
  mov eax, dword [myArray+DARRAY.ptr]
  ret;

;----------Get random Numbers------------;
next_random_60:
		push ebp
    mov ebp, esp
    pushad
    call next_Random    

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num120] ;push single_value2 to fpu stack(st0)
    fmul   
    fild    dword [num60] ;push single_value2 to fpu stack(st0)
    fsub
    fstp	qword [out100Random] 

    fld qword[out100Random] 
    fld qword[num60]
    fcomi
    jc above60
    jmp endr

    above60:
    fld qword[num601] 
    fstp qword[out100Random]

    endr:
    popad
    mov esp, ebp
    pop ebp 
		ret


next_random_10:
		push ebp
    mov ebp, esp
    pushad
    call next_Random    

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num20] ;push single_value2 to fpu stack(st0)
    fmul   
    fild    dword [num10] ;push single_value2 to fpu stack(st0)
    fsub
    fstp	qword [out100Random]  
  ;  my_print_float [out100Random+4], [out100Random], msg
      
    popad
    mov esp, ebp
    pop ebp 
		ret


next_XOrY:
		push ebp
    mov ebp, esp
    pushad
    call next_Random    

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num100] ;push single_value2 to fpu stack(st0)
    fmul   
    fstp	qword [out100Random]  
     
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
    fcos
    fld    qword [speed] 
    fmul
    fld    qword [x] 
    fadd        
    fstp	qword [x]  
 

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

    fld	qword [y]
    fild	dword [num100] 
    
    fcomi
    jc .sub100    
 ;   jmp .endSub100

    fld	qword [num0]
    fld	dword [y] 
  
    fcomi
    jc .sub0  
    jmp .endSub100
    
    .sub100:
    fstp	qword [y]  
    fld	qword [y]
    fsub
    fstp	qword [y]    

     .sub0:
    fld	qword [num0]
    fstp	dword [y] 
       

    .endSub100:
 
    popad
    mov esp, ebp
    pop ebp 
		ret 

next_random_360:
		push ebp
    mov ebp, esp
    pushad
    call next_Random    

    fild	dword [seed]	        ; need to convert 32-bit to 64-bit
	  fild	dword [maxInt]
	  fdiv
    fild    dword [num360] ;push single_value2 to fpu stack(st0)
    fmul   
    fld    dword [num0] ;push single_value2 to fpu stack(st0)
    fsub
    fstp	qword [out100Random]  
  ;  my_print_float [out100Random+4], [out100Random], msg
      
    popad
    mov esp, ebp
    pop ebp 
		ret
    
next_angle:
   	push ebp
    mov ebp, esp
    pushad
    finit
    call next_random_60


    fld qword[engal]
    fld qword[out100Random]
    fsub

    fstp	qword [engal]

    fld qword[engal]
    fild dword[num360]
    fcomi
    jc .above100    
    

    fld qword[num0]
    fld qword[engal]
    fcomi
    jc .under0    
    jmp .endSub360
  
    .above100:
    fild dword[num360] 
    fstp	qword [engal] 
    jmp .endSub360

    .under0:
    fld qword[num0] 
    fstp	qword [engal] 
    jmp .endSub360


    .endSub360:
  
    popad
    mov esp, ebp
    pop ebp 
		ret

next_speed:

   	push ebp
    mov ebp, esp
    pushad
    finit
    call next_random_10
    fld qword[speed]
    fld qword[out100Random]
    fadd
    fstp	qword [speed]  
    fld qword[speed]
    fild dword[num100] 
    fcomi
    jc .above100    
    
    fld qword[num0]
    fld qword[speed]
    fcomi
    jc .underspeed    
    jmp .endspeed
  

    .above100:
    fild dword[num100] 
    fstp	qword [speed] 
    jmp .endspeed

    .underspeed:
    fld qword[num0] 
    fstp	qword [speed] 
    jmp .endspeed

    .endspeed:  
    popad
    mov esp, ebp
    pop ebp 
		ret

check_winner:
 mov dword[counter], 0
 mov dword[winner], 0

  loopwinner:
  mov edx, dword[counter]
  cmp edx , dword[N] ; N is the number of co-routines
  jge endwinner
  mov eax, dword [myArray + DARRAY.ptr]
  imul edx, 40 ; 0- X 8-Y 16-speed 24- Angle 32-Target 36- Alive 
	lea edx, [eax + edx]
  cmp dword[edx+36], 1
  jne inccounter
  mov ecx, dword[maxtargets]
  cmp [edx+36], ecx 
  jle inccounter
  mov ecx, dword[counter]
  mov dword[winner], ecx
  mov eax, dword[edx+36]
  mov dword[maxtargets], eax

inccounter:
  inc dword[counter]
  jmp loopwinner

endwinner:
my_print dword[winner], winnerfrm
ret
   