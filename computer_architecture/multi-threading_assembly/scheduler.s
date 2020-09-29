   global startCo
   global initCo
   global do_resume
   extern printf
   extern CORS
   extern checkaliminatenew
   extern N
 ;  extern endCo
   extern function_cors
   global schedulerfunction
   extern K
   extern R
   extern printer
   extern printerrfunction
   global resume
   extern COi
   extern schedule
   extern target
   extern targetfunction
   extern numberdrones
   extern destroyedcore
   extern checkaliminate
   extern get_my_loc
   global countercor
   global rounds
   global continueprint
   extern endofthegame
   global inccounter

section .data
    stringFormat    : db "%s",10,0
	message         : db "scheduler is here", 0
    message2         : db "arrive to the end of the function !!", 0
	message1         : db "We are on schedule function !!", 0
    end         : db "This is the end of the gameee", 0
    intFormat     : db "%d",10,0 
    roundspr     : db "The rounds are =%d",10,0 
    alive     : db "The drones that alive are =%d",10,0 
    SPMAIN          : dd 0
    CURR            : dd 0
    SPP	            : dd 4     ; offset of pointer to co-routine stack in co-routine struct 
	CODEP	        : dd 0	     ; offset of pointer to co-routine function in co-routine struct 
	SPT	            : dd 0     ; offset of pointer to co-routine stack in co-routine struct 
	counter         : dd 0
    rounds          : dd 0
    countercor      : dd 0

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


schedulerfunction:
 mov dword[rounds], 0
 mov edx, dword[N]

 begin:

 mov eax, dword[countercor]
 cmp eax, dword[N]
 jne loopscheduler
 mov dword[countercor], 0


 loopscheduler:
   cmp dword[numberdrones], 1
   je endfunction
   xor EBX, EBX
   xor eax, eax
   xor edx, edx
   mov eax, dword[rounds]
   mov ecx, dword[N]
   mov edx, 0
   idiv ecx
   add edx, 1
   call get_my_loc
   imul edx, 40 ; index * 20
   lea edx, [eax + edx]
   mov ecx, [edx+32]
   cmp ecx, 0
   jne activatecor

   activatecor:
   mov edx, dword[countercor]
   mov EBX, COi
   call resume

   continueprint:              ; check if it is the time to print
   mov edx, 0
   mov eax, dword[rounds]
   idiv dword[K]
   cmp edx, 0
   jne inccounter
   mov EBX, printer
   call resume 

   continue_eliminate:
   mov eax, dword[rounds] ; check if ((i%N)==0) && ((i/N)%R)==0
   mov edx,0
   idiv dword[N]
   cmp edx, 0
   jne inccounter
   push dword[rounds]
   call checkaliminate
 ;  sub dword[numberdrones], 1

   inccounter:
   add dword[rounds], 1
   add dword[countercor], 1
   jmp begin

endfunction:
  my_print end, stringFormat
  jmp endofthegame


endCo:
  mov ESP, [SPMAIN]   ; restore ESP of main()
  popad	
  mov esp, ebp
  pop ebp	
  ret

resume:	; save state of current co-routine
	pushfd
	pushad
	mov EDX, [CURR]
	mov [EDX+4], ESP   ; save current ESP
do_resume:  ; load ESP for resumed co-routine
	mov ESP, [EBX+4]
	mov [CURR], EBX
	popad  ; restore resumed co-routine state
	popfd
	ret        

