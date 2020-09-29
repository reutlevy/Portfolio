
section .data
    calcFormat     : db "%s",0  
    stringFormat     : db "%s",10,0                         
    intFormat     : db "%d",10,0 
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
               

section .bss
    ; op_stack :    resb 1  
    buffer :    resb 80   
    buffer_length equ $ - buffer
    bufferToCopy :    resb 80    
        
   
%macro my_push 1
    push ebp
    mov ebp, esp
    pushad
    mov ebx, %1    
    mov eax, [stack_pointer]    
    imul eax, 4      
    mov edx, [op_stack]    
    add edx, eax               
    mov [edx] , dword ebx
    inc byte [stack_pointer]
    popad
    mov esp, ebp
    pop ebp
  ;  add word[stack_pointer], 5; -- the stack pointer noe changed
%endmacro

%macro my_pop 0
    push ebp
    mov ebp, esp
    pushad
    dec byte [stack_pointer]        
    mov eax, [stack_pointer]    
    imul eax, 4      
    mov edx, [op_stack]    
    add edx, eax  
    mov edx , dword [edx]                 
    mov [pop_value] , dword edx    
    popad
    mov esp, ebp
    pop ebp
  ;  add word[stack_pointer], 5; -- the stack pointer now changed
%endmacro

%macro my_peek 0
    push ebp
    mov ebp, esp
    pushad
    mov ecx, [stack_pointer]
    dec ecx        
    mov eax, ecx    
    imul eax, 4      
    mov edx, [op_stack]    
    add edx, eax  
    mov edx , dword [edx]                 
    mov [dup_value] , dword edx    
    popad
    mov esp, ebp
    pop ebp
  ;  add word[stack_pointer], 5; -- the stack pointer noe changed
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

%macro my_print_error 2
    push ebp
    mov ebp, esp
    pushad
    mov ecx, %1
    mov edx, %2
    push ecx;
    push edx
    push dword[stderr]
    call fprintf    
    add esp, 12
    popad
    mov esp, ebp
    pop ebp  
%endmacro

%macro add_first 1
    push ebp
    mov ebp, esp  
    pushad  
    ; push 5;
    ; call malloc; 
    ; add esp, 4 
    safe_malloc 5 
    mov ebx, %1       
    mov [first_link], eax       
    mov [curr_link], eax   
    mov [last_link], eax            
    mov [eax], byte bl   
    ; my_print [eax], intFormat 
    mov dword[eax+4], 0  
    popad       
    mov esp, ebp
    pop ebp
  ;  my_push eax
  ;  my_push eax; -- add the new link pointer to the stack
%endmacro

%macro new_link 1;    
    push ebp
    mov ebp, esp    
    ; push 5;
    ; call malloc; 
    ; add esp, 4    
    safe_malloc 5       
    mov [last_link], eax            
    mov ebx, %1        
    mov [eax], byte bl    
    mov dword[eax+4], 0 
    mov edx, [curr_link]   
    mov dword[edx+4], eax
    mov [curr_link], eax            
    mov esp, ebp
    pop ebp
  ;  my_push eax
  ;  my_push eax; -- add the new link pointer to the stack
%endmacro

%macro firstToInt 0
    push ebp
    mov ebp, esp
    mov ebx, bufferToCopy
    mov ecx, 0
    mov edx, [bufferCounter]    
    cmp dword edx, [buffer_size]
	  jge %%endSecond
    mov dword [firstCharVal], 0

	cmp byte [ebx+edx], '0' 
	jb %%checkBig
	cmp byte [ebx+edx], '9'
	ja %%checkBig
	
	movzx ecx, byte [ebx+edx]
	sub ecx, '0'
    mov [firstCharVal], ecx
    jmp %%endBig

    %%checkBig:

    cmp byte [ebx+edx], 'A' 
	jb %%endBig
	cmp byte [ebx+edx], 'F'
	ja %%endBig

    movzx ecx, byte [ebx+edx]
	sub ecx, 55
    mov [firstCharVal], ecx
    %%endBig:  
    %%endSecond: 	
    inc byte [bufferCounter]    
    mov esp, ebp
    pop ebp
%endmacro

%macro secondToInt 0
    push ebp
    mov ebp, esp
    mov ebx, bufferToCopy
    mov ecx, 0
    mov edx, [bufferCounter]
    cmp dword edx, [buffer_size]
	  jge %%endSecond
    mov dword [secondCharVal], 0

	cmp byte [ebx+edx], '0' 
	jb %%checkBig
	cmp byte [ebx+edx], '9'
	ja %%checkBig
	
	movzx ecx, byte [ebx+edx]
	sub ecx, '0'
    mov [secondCharVal], ecx
    jmp %%endBig

    %%checkBig:

    cmp byte [ebx+edx], 'A' 
	jb %%endBig
	cmp byte [ebx+edx], 'F'
	ja %%endBig

    movzx ecx, byte [ebx+edx]
	sub ecx, 55
    mov [secondCharVal], ecx
    %%endBig:   
    %%endSecond:	
    inc byte [bufferCounter]    
    mov esp, ebp
    pop ebp
%endmacro


section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern fflush
  extern malloc 
  extern calloc 
  extern free 
  extern gets 
  extern getchar 
  extern fgets 
  global _start
  extern stdin  
  extern stderr

  
main:
    
    push ebp
    mov ebp, esp 

    ; Args flag
    call check_Args        
    ;start stack
    call InitStack
    call menu

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
    je notPrint

    xor edx, edx
    xor eax, eax

    mov eax, dword[ebx]
    movzx edx, byte [eax] ; get a character 
    cmp edx, '-'
    jne number
    mov dword[DFlag], 0x01


    number:
    push dword[ebx]
    call convert_number_args
    add esp, 4
    jmp notPrint

    notPrint:
     
    inc ecx 
    add ebx, 4 ; get next arg
    cmp ecx, dword[num_of_args] ; comp index of args to args number
    jne loopArgs ; jamp if we didnt get to the end

    ret

convert_number_args:

  push ebp
  mov ebp, esp
  pushad
  mov ecx, dword [ebp+8]	; get function argument (pointer to string)
  xor eax, eax ; 
  xor edx, edx ; 

  covert_number:
      movzx ebx, byte [ecx] ; get a character
      cmp ebx, '-' ;; know that it isnt debug
      je not_number
      inc ecx ; promote ecx
      cmp ebx, 0 ; check if null terminated
      jz finally;
      cmp ebx, 0x71 ; check if q
      jz finally ;
      sub ebx, 48 ; "convert" character to number
      imul eax, 0x0A ; multiply result by ten
      add eax, ebx ; add the current digit
      jmp covert_number ;

    finally:
      mov dword[stack_size], eax
      popad 
      mov esp, ebp
      pop ebp
      ret

    not_number:
      popad 
      mov esp, ebp
      pop ebp
      ret

debug:
  push ebp
  mov ebp, esp
  pushad
  mov ecx, dword [ebp+8]	; get function argument (pointer to string) 
  movzx ebx, byte [ecx] ; get a character 
  cmp ebx, '-'
  jne done_debug
  mov dword[DFlag], 0x01

  done_debug:
  popad 
  mov esp, ebp
  pop ebp
  ret

getInput: 
        push ebp
        mov ebp, esp
        pushad
        ; get 1 input
        push dword [stdin]
        push buffer_length
        push buffer           ; 3 pushes gets the stack back to 16B-alignment
        call fgets
        add esp, 12
        ;end user input

        popad
        mov esp, ebp
        pop ebp
        ret
changeBufferDirection:
        push ebp
        mov ebp, esp
        pushad
        ;start change string dirction
        mov byte [buffer_size], 0
        mov ebx, buffer
        Loop:
        inc byte [buffer_size]
        inc ebx    
        cmp byte [ebx], 0 
        je endLoop
        cmp byte [ebx], 10 
        je endLoop
        jmp Loop
        endLoop:

        mov ecx, bufferToCopy
        dec ebx
        Loop3:

        mov edx, [ebx] 
        mov [ecx], edx	
        inc ecx
        dec ebx
        cmp byte [ebx], 0 
        jne Loop3        
        mov byte [ecx] ,0
        ;end change dirction
        popad
        mov esp, ebp
        pop ebp
        ret

makeList:
    push ebp
    mov ebp, esp
    pushad    

    ; call getInput  

    mov byte [curr_link],0 
    mov byte [first_link],0  
    mov byte [last_link],0  
    ;buffer to buffer copy
    call changeBufferDirection  
    mov dword [bufferCounter],0

    mov dword [firstCharVal],0
    mov dword [secondCharVal],0
    mov dword [nodeVal],0     
    firstToInt        
    secondToInt
    mov edx, dword [secondCharVal]
    mov ecx, dword [firstCharVal]
    imul edx, 16 ; multiply "result so far" by ten
    add ecx, edx ; add in current digit	 
    mov [nodeVal], dword ecx        
    mov ebx,dword [nodeVal]

    add_first ebx    

    LoopList:
        mov edx, [bufferCounter]        
        cmp dword edx, [buffer_size]
        jge EndLoopList

        mov dword [firstCharVal],0
        mov dword [secondCharVal],0
        mov dword [nodeVal],0

        firstToInt        
        secondToInt
        mov edx, dword [secondCharVal]
        mov ecx, dword [firstCharVal]
        imul edx, 16 ; multiply "result so far" by ten
        add ecx, edx ; add in current digit	 
        mov [nodeVal], dword ecx        
        mov ebx,dword [nodeVal]

        new_link ebx
        jmp LoopList

    EndLoopList:

    popad
    mov esp, ebp
    pop ebp

    ret

InitStack:
    push ebp
    mov ebp, esp
    pushad
    mov eax, [stack_size]
    imul eax, 4     
    safe_malloc eax    
    mov dword [op_stack], eax 
    popad
    mov esp, ebp
    pop ebp
    ret
           

pop_print: 
        push ebp
        mov ebp, esp
        pushad        
        mov eax, [pop_value]                
        cmp eax, 0 
        je endPop     
        ; my_print [eax], intFormat   
        mov ebx, eax        
        add ebx, 4
        mov ebx,[ebx]
        ; my_print ebx, intFormat
        mov [pop_value], ebx
        call pop_print
        my_print [eax], hexFormat       

        endPop:

        popad
        mov esp, ebp
        pop ebp
        ret

dup: 
        push ebp
        mov ebp, esp
        pushad   
        mov byte [curr_link],0 
        mov byte [first_link],0  
        mov byte [last_link],0           
        mov eax, [dup_value]                
        cmp eax, 0         
        je endDup
        mov ebx, [eax]        
        add_first ebx
        DupLoop:       
        ; my_print [eax], intFormat           
        mov ebx, [dup_value]        
        add ebx, 4
        mov ebx,[ebx]
        cmp ebx, 0         
        je endDup                    
        mov [dup_value], ebx        
        new_link [ebx]        

        jmp DupLoop  

        endDup:

        popad
        mov esp, ebp
        pop ebp
        ret
sum: 
        push ebp
        mov ebp, esp
        pushad   
        mov byte [curr_link],0 
        mov byte [first_link],0  
        mov byte [last_link],0 
        mov byte [sum_curry],0          

        mov eax, [pop_value]                
        cmp eax, 0         
        je num1Zero
        mov eax, [eax]
        num1Zero:
        

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je num2Zero
        mov ebx, [ebx]
        num2Zero:        

        add ebx, eax       
        cmp ebx, 256         
        jb add_Node
        sub ebx, 255
        mov byte [sum_curry], 1
        

        add_Node:
        add_first ebx

        mov eax, [pop_value]                
        cmp eax, 0         
        je nextnum1
        add eax, 4
        mov eax, [eax]
        mov [pop_value], eax
        nextnum1:

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je nextnum2
        add ebx, 4
        mov ebx, [ebx]
        mov [pop_value2], ebx
        nextnum2:

        
        sumLoop:               
        mov eax, [pop_value]                
        cmp eax, 0         
        jne loopOk

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je endsumLoop
        loopOk:

        mov eax, [pop_value]                
        cmp eax, 0         
        je num1Zero2
        mov eax, [eax]
        num1Zero2:

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je num2Zero2
        mov ebx, [ebx]
        num2Zero2:

        add ebx, eax
        add ebx, [sum_curry]
        mov byte [sum_curry], 0
        cmp ebx, 256         
        jb add_Node2
        sub ebx, 255
        mov byte [sum_curry], 1

        add_Node2:
        new_link ebx

        mov eax, [pop_value]                
        cmp eax, 0         
        je nextnum12
        add eax, 4
        mov eax, [eax]
        mov [pop_value], eax
        nextnum12:

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je nextnum22
        add ebx, 4
        mov ebx, [ebx]
        mov [pop_value2], ebx
        nextnum22:
        jmp sumLoop        

        endsumLoop:

        popad
        mov esp, ebp
        pop ebp
        ret

or_func: 
        push ebp
        mov ebp, esp
        pushad   
        mov byte [curr_link],0 
        mov byte [first_link],0  
        mov byte [last_link],0                 

        mov eax, [pop_value]                
        cmp eax, 0         
        je num1Zero66
        mov eax, [eax]
        num1Zero66:
        

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je num2Zero66
        mov ebx, [ebx]
        num2Zero66:        

        or eax, ebx
        mov ebx,eax

        add_first ebx

        mov eax, [pop_value]                
        cmp eax, 0         
        je nextnum166
        add eax, 4
        mov eax, [eax]
        mov [pop_value], eax
        nextnum166:

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je nextnum266
        add ebx, 4
        mov ebx, [ebx]
        mov [pop_value2], ebx
        nextnum266:

        
        orLoop:               
        mov eax, [pop_value]                
        cmp eax, 0         
        jne loopOk22

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je endorLoop
        loopOk22:

        mov eax, [pop_value]                
        cmp eax, 0         
        je num1Zero277
        mov eax, [eax]
        num1Zero277:

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je num2Zero277
        mov ebx, [ebx]
        num2Zero277:

        or eax, ebx
        mov ebx, eax

        new_link ebx

        mov eax, [pop_value]                
        cmp eax, 0         
        je nextnum1277
        add eax, 4
        mov eax, [eax]
        mov [pop_value], eax
        nextnum1277:

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je nextnum2277
        add ebx, 4
        mov ebx, [ebx]
        mov [pop_value2], ebx
        nextnum2277:
        jmp orLoop        

        endorLoop:

        popad
        mov esp, ebp
        pop ebp
        ret
and: 
        push ebp
        mov ebp, esp
        pushad   
        mov byte [curr_link],0 
        mov byte [first_link],0  
        mov byte [last_link],0                   

        mov eax, [pop_value]                
        cmp eax, 0         
        je iszero
        mov eax, [eax]
        jmp notzero

        iszero:
        add_first 0
        jmp endAndLoop
        notzero:
        

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je iszero
        mov ebx, [ebx]                

        and eax, ebx 
        mov ebx, eax             
        add_first ebx

        mov eax, [pop_value]                
        cmp eax, 0         
        je endAndLoop
        add eax, 4
        mov eax, [eax]
        mov [pop_value], eax        

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je endAndLoop
        add ebx, 4
        mov ebx, [ebx]
        mov [pop_value2], ebx        

        
        andLoop:               
        mov eax, [pop_value]                
        cmp eax, 0         
        je endAndLoop

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je endAndLoop
        ;;;;
        mov eax, [pop_value]                
        cmp eax, 0         
        je endAndLoop
        mov eax, [eax]        

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je endAndLoop
        mov ebx, [ebx]
        

        and eax, ebx
        mov ebx, eax             
        new_link ebx      
        

        mov eax, [pop_value]                
        cmp eax, 0         
        je endAndLoop
        add eax, 4
        mov eax, [eax]
        mov [pop_value], eax        

        mov ebx, [pop_value2]                
        cmp ebx, 0         
        je endAndLoop
        add ebx, 4
        mov ebx, [ebx]
        mov [pop_value2], ebx
        
        jmp andLoop        

        endAndLoop:

        popad
        mov esp, ebp
        pop ebp
        ret

numLentgh: 
        push ebp
        mov ebp, esp
        pushad   
        mov byte [curr_link],0 
        mov byte [first_link],0  
        mov byte [last_link],0 
        mov byte [number_lentgh],0 

        mov eax, [pop_value]                
        cmp eax, 0         
        je endLentgh        
        LentghLoop:       
        ; my_print [eax], intFormat           
        mov ebx, [pop_value]        
        add ebx, 4
        mov ebx,[ebx]
        cmp ebx, 0         
        je LastNode
        mov ecx, dword [number_lentgh]   
        add ecx, 2                 
        mov [number_lentgh], ecx 
        mov [pop_value], ebx
        jmp LentghLoop
        LastNode:  
        mov ebx, [pop_value]
        cmp byte [ebx], 16 
        jge FluseTwo
        mov ecx, dword [number_lentgh]   
        add ecx, 1                 
        mov [number_lentgh], ecx
        jmp endFluseTwo

        FluseTwo:
        mov ecx, dword [number_lentgh]   
        add ecx, 2                 
        mov [number_lentgh], ecx
        endFluseTwo:  

        mov eax, dword [number_lentgh]
        mov dword [firstCharVal],0
        mov dword [secondCharVal],0
        mov dword [nodeVal],0 

        cmp eax, 0 
        je endLentgh  


        mov edx, dword 0	
        mov ecx, dword 16 
        DIV ecx	
        ; my_print edx, intFormat

        mov [firstCharVal], edx
        cmp eax, 0 
        je createNode

        mov edx, dword 0	
        mov ecx, dword 16 
        DIV ecx	
        mov [secondCharVal], edx          

        createNode:
        mov [number_lentgh], eax
        mov edx, dword [secondCharVal]
        mov ecx, dword [firstCharVal]
        imul edx, 16 ; multiply "result so far" by ten
        add ecx, edx ; add in current digit	 
        mov [nodeVal], dword ecx        
        mov ebx,dword [nodeVal]

        add_first ebx

        
        LentghLoop2:
        mov eax, dword [number_lentgh]
        mov dword [firstCharVal],0
        mov dword [secondCharVal],0
        mov dword [nodeVal],0 

        cmp eax, 0 
        je endLentgh  


        mov edx, dword 0	
        mov ecx, dword 16 
        DIV ecx	

        mov [firstCharVal], edx
        cmp eax, 0 
        je createNode2

        mov edx, dword 0	
        mov ecx, dword 16 
        DIV ecx	
        mov [secondCharVal], edx          

        createNode2:
        mov [number_lentgh], eax
        mov edx, dword [secondCharVal]
        mov ecx, dword [firstCharVal]
        imul edx, 16 ; multiply "result so far" by ten
        add ecx, edx ; add in current digit	 
        mov [nodeVal], dword ecx        
        mov ebx,dword [nodeVal]

        new_link ebx
        jmp LentghLoop2

           

        endLentgh:

        popad
        mov esp, ebp
        pop ebp
        ret

free_memory:
    push ebp
    mov ebp, esp
    pushad

    
   .loop_free:
   cmp dword[stack_pointer], 0x00
   je end_free
   my_pop
   mov eax, dword[pop_value]
   mov [pop_free1], eax
   push dword[pop_free1]
   call free
   add esp, 4
   jmp .loop_free
   
    end_free: 
    push dword[op_stack]
    call free
    add esp, 4

    popad
    mov esp, ebp
    pop ebp
    ret
  
   

menu:
    push ebp
    mov ebp, esp
    pushad
    StartMenu:
    cmp dword[DFlag], 0x01
    je debug_print
    jne start_menu

    debug_print:
    my_print [stack_pointer], stack_pointer_print

    start_menu:
    inc dword[counter]
    my_print calc, calcFormat 
    call getInput  
    mov ecx, buffer        
    cmp byte [ecx], 'q' 
	  je StartQ

    cmp byte [ecx], '+' 
	  je StartPlus

    cmp byte [ecx], 'p' 
	  je StartPrint

    cmp byte [ecx], 'd' 
	  je StartDup

    cmp byte [ecx], '&' 
	  je StartAnd

    cmp byte [ecx], '|' 
	  je StartOr

    cmp byte [ecx], 'n' 
	  je StartN

    cmp byte [ecx], '*' 
	  je StartMul

    StartCreateNumber:
    mov eax, [stack_pointer]
    mov ebx, [stack_size]
    cmp eax, ebx
    jge error_stack    
    call makeList
    mov eax, dword [first_link]  
    my_push eax    
    mov dword [buffer_size],0  
    mov dword [firstCharVal],0
    mov dword [secondCharVal],0
    mov dword [nodeVal],0         
    EndCreateNumber:
    jmp StartMenu

    StartMul:

    EndMul:
    jmp StartMenu

    StartN:
    cmp dword[stack_pointer], 0x0
    je error_regular
    my_pop
    mov eax, [pop_value]
    mov [pop_free1], eax
    call numLentgh
    ; my_print [number_lentgh], intFormat
    mov eax, dword [first_link]  
    my_push eax
    push dword[pop_free1]
    call free
    add esp, 4
    EndN:
    jmp StartMenu


    StartOr:
    cmp dword[stack_pointer], 0x02
    jb error_regular
    my_pop
    mov dword eax, [pop_value]
    mov dword [pop_value2], eax
    mov dword [pop_free1], eax
    my_pop
    mov dword eax, [pop_value]
    mov dword [pop_free2], eax
    call or_func
    mov eax, dword [first_link]  
    my_push eax
    cmp dword[DFlag], 0x01
    jne next_or
    my_print_error [eax], hexFormatnew

    next_or:
    push dword[pop_free1]
    call free
    add esp,4
    push dword[pop_free2]
    call free
    add esp,4
    EndOr:
    jmp StartMenu


    StartAnd:
    cmp dword[stack_pointer], 0x02
    jb error_regular
    my_pop
    mov dword eax, [pop_value]
    mov dword [pop_value2], eax
    mov dword [pop_free1], eax
    my_pop
    mov dword eax, [pop_value]
    mov dword [pop_free2], eax
    call and
    mov eax, dword [first_link]
    cmp dword[DFlag], 0x01
    jne next_and
    my_print_error [eax], hexFormatnew

    next_and:
    my_push eax
    push dword[pop_free1]
    call free
    add esp,4
    push dword[pop_free2]
    call free
    add esp,4
    EndAnd:
    jmp StartMenu

    StartDup:
    cmp dword[stack_pointer], 0x01
    jb error_regular
    mov eax, [stack_pointer]
    mov ebx, [stack_size]
    cmp eax, ebx
    jge error_stack  
    my_peek
    mov eax,[dup_value]      
    call dup
    mov eax, dword [first_link] 
    cmp dword[DFlag], 0x01
    jne next_dup
    my_print_error [eax], hexFormatnew

    next_dup: 
    my_push eax 
    EndDup:
    jmp StartMenu

    StartPrint:
    cmp dword[stack_pointer], 0x01
    jb error_regular
    my_pop 
    mov dword eax, [pop_value] 
    mov [pop_free2], eax
    call pop_print
    my_print "",newLine  
    push dword[pop_free2]
    call free
    add esp,4
    EndPrint:
    jmp StartMenu


    StartPlus:
    cmp dword[stack_pointer], 0x02
    jb error_regular
    my_pop
    mov dword eax, [pop_value]
    mov dword [pop_value2], eax
    mov dword [pop_free1], eax
    my_pop
    mov dword eax, [pop_value]
    mov dword [pop_free2], eax
    call sum
    mov eax, dword [first_link]  
    my_push eax
    cmp dword[DFlag], 0x01
    jne next_plus
    my_print_error [eax], hexFormatnew

    next_plus:  
  ;  my_push eax
    push dword[pop_free1]
    call free
    add esp,4
    push dword[pop_free2]
    call free
    add esp,4
    EndPlus:
    jmp StartMenu

    error_regular:
    my_print_error 0, error_args
    jmp StartMenu

    error_stack:
    my_print_error 0, stack_overflow
    jmp StartMenu   

    StartQ:
    dec dword[counter]
  ;  mov eax, dword[counter]
    my_print dword[counter], hexFormat1
    call free_memory

    EndQ:
    
    EndMenu:

    popad
    mov esp, ebp
    pop ebp
    ret

  



