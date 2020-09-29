 global function_cors
 extern printf
 extern schedule
 extern resume
 extern Xtarget
 extern x
 extern y
 extern numberdrones
 extern N
 extern out100Random
 extern next_angle
 extern next_speed
 extern speed
 extern engal
 extern new_X
 extern new_Y
 extern get_my_loc
 global R
 extern Ytarget
 extern Ytaget
 extern Speedtarget
 extern continueprint
 global destroyedcore
 extern Angletarget
 global checkactive
 global checkaliminate
 extern targetfunction
 extern target
 extern inccounter
 extern d
 extern rounds
 extern countercor
 global endfunctioncors
 global checkaliminatenew

section .data
    stringFormat     : db "%s",10,0                         
    intFormat     : db "The destroyed core issss %d",10,0 
    intFormat1     : db "Counter is %d",10,0 
    TintFormat     : db "Number of thargets destroyed =%d",10,0 
    DintFormat     : db "Number of drones alive  =%d",10,0 
    floatFormat     : db "the x value isssss %f",10,0 
    floatFormat1     : db "the x = %f",10,0 
    floatdis        : db "The min distance is =%f",10,0 
    floatdis1        : db "The cur distance is =%f",10,0 
    Caldis        : db "Cal dissss = %d",10,0 
    retfromcal        : db "Return from cal = %d",10,0 
    intFormatSscanf     : db "%d",0 
    floatFormatSscanf     : db "%e",0 
    FMT1: db "Function1, co %d", 10, 0 
    msg     db      "sum = %f",10,0
    message         : db "function in process", 0
    message1         : db "We are on Cors function", 0
    message2        : db "Back from the drones function", 0
    counter         : dd 0
    mintar          : dd 0
    mindis          : dq 0.0
    cmpdis          : dq 0
    destroyedcore   : dd 0

    xOut	dq	0.0
    num0	dq	0.0
    yOut	dq	0.0

    distance dq	0.0
 
section .text


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

function_cors:
        
        call get_my_loc
        mov edx, dword[countercor]
        imul edx, 40 ; index * 20
        lea edx, [eax + edx]

      ;---- get x new value----;
        fld   qword [edx]   
        fstp   qword [x]
        call new_X
        fld   qword [x] 
        fstp   qword [edx]

    ;---- get y new value----; 
        fld   qword [edx+8]   
        fstp   qword [y]
        call new_Y
        fld   qword [y] 
        fstp   qword [edx+8]

      ;---- get speed new value----; 
        fld   qword [edx+16]   
        fstp   qword [speed]
        call next_speed
        fld   qword [speed] 
        fstp   qword [edx+16]

     ;---- get angle new value----; 
        fld   qword [edx+24]   
        fstp   qword [engal]
        call next_angle
        fld   qword [engal] 
        fstp   qword [edx+24]
     
     ;----- check we can destroy the target----;
      fld   qword [edx]   
      fstp   qword [x]
      fld   qword [edx+8]   
      fstp   qword [y]


      continuedes:
      call Calculate_distance
      fld   qword [d]
      fld   qword [distance]
      fcomi
      jc aliminate_target  
      jmp endfunctioncors

      aliminate_target:
      add dword[edx+36], 1
      mov EBX, target
      call resume
      jmp endfunctioncors


endfunctioncors:
  mov EBX, schedule
  call resume
  jmp function_cors

checkaliminate:
   push ebp
   mov ebp, esp 
   call get_my_loc
   lea edx, [eax]
   mov dword[counter], 1
   cmp dword[edx+32], 0
   je loopcors
   lea edx, [eax]
   fld  qword[edx]
   fstp  qword[x]
   fld qword[edx+8]
   fstp  qword[y]
   call Calculate_distance
   fld qword[distance]
   fstp  qword[mindis]
   mov dword[destroyedcore], 0

   loopcors:
    mov edx, dword[counter]
    cmp edx, dword[N]
    je endeliminate
    imul edx, 40 ; index * 20
    lea edx, [eax+edx]
    cmp dword[edx+32], 0
    je inc_counter
    fld  qword[edx]
    fstp  qword[x]
    fld qword[edx+8]
    fstp  qword[y]
    call Calculate_distance
    fld  qword[mindis]
    fld  qword[distance]
    fcomi
    jc continue
    jmp inc_counter

    continue:
    fld qword[distance]
    fstp  qword[mindis]
   ; mov dword[destroyedcore], ebx

    inc_counter:
    inc dword[counter]
    jmp loopcors

endeliminate:
 sub dword[numberdrones], 1; we dec the number of drones that has left
 xor eax, eax
 call get_my_loc 
 mov edx, dword[destroyedcore]  
 imul edx, 40
 lea edx, [eax + edx]
 mov dword[edx + 32], 0 ; change active status to zero
 mov esp, ebp
 pop ebp
 jmp inccounter  


Calculate_distance:   
   ; finit
    fld	qword [x]    
    fld	qword [Xtarget]	
	  fsub
    fstp	qword [xOut]      
    fld	qword [xOut]	  
    fld	qword [xOut]	      
    fmul
    fstp	qword [xOut]      
    fld	qword [y]    
    fld	qword [Ytarget]	  
	  fsub
    fstp	qword [yOut]      
    fld	qword [yOut]	  
    fld	qword [yOut]	      
    fmul
    fstp	qword [yOut]      
    fld	qword [xOut]	  
    fld	qword [yOut]	      
    fadd
    fsqrt
    fstp	qword [distance]  

    end:
    ret  


checkaliminatenew:
   push ebp
   mov ebp, esp 
   call get_my_loc
   lea edx, [eax]
   mov dword[counter], 1
   cmp dword[edx+32], 0
   je loopcors1
   lea edx, [eax]
   mov eax, dword[edx+36] ; eax now holds the number of thargets that has been destroyed
   mov dword[mintar], eax

   loopcors1:
    mov edx, dword[counter]
    cmp edx, dword[N]
    je endelimination
    imul edx, 40 ; index * 20
    lea edx, [eax+edx]
    cmp dword[edx+32], 0 ; check if the drone alive
    je inc_count
    mov eax, dword[edx+36] ; eax now holds the number of thargets that has been destroyed
    cmp dword[mintar], eax
    jle findmin ; if it is destroyed the smallest number 

    findmin:
    mov eax, dword[edx+36]
    mov dword[mintar], eax
    mov edx, dword[counter]
    mov dword[destroyedcore], edx ; the id of the drone that has been destroyed

    inc_count:
    inc dword[counter]
    jmp loopcors1

endelimination:
 sub dword[numberdrones], 1; we dec the number of drones that has left
 xor eax, eax
 call get_my_loc 
 mov edx, dword[destroyedcore]  
 imul edx, 40
 lea edx, [eax + edx]
 mov dword[edx + 32], 0 ; change active status to zero
 mov esp, ebp
 pop ebp
 jmp inccounter 

    
