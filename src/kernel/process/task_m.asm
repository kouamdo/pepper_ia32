global switch_to_task

extern  set_tcb,CURRENT_TASK , memcpy , sizeof
extern size_tcb

section .text


	;In C declaration , : switch_to_task(task_control_block_t current_task)
	switch_to_task:
		
		;save previous task's state
		push ebx
		push esi
		push edi
		push ebp

		mov edi ,CURRENT_TASK	;address of the current task
		mov dword[edi] , esp	;Save ESP for previous task's in the current stask's task

		;load next task's state
		mov esi , dword [CURRENT_TASK+12]	;address of the next TCB
		
		;copy memmory of current_task.................
			push dword [size_tcb]
			push esi
			push CURRENT_TASK
			call memcpy
			pop eax
			pop eax
			pop eax
	; 	;end of the copy..............................
	
		mov esp , dword [CURRENT_TASK] ;load esp next task's in esp
		mov eax , dword [CURRENT_TASK+8];address of page directory
		mov ebx , dword [CURRENT_TASK+4];top of the stack's task
		mov ecx , cr3
		cmp ecx , eax
		je .allows

	.allows:
		pop ebp
		pop edi
		pop esi
		pop ebx