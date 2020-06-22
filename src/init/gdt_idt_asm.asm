global load_idt , load_gdt


gdtr DW 0	;For limit
	DD 0	;For Base

section .text

load_gdt:
	cli
	push eax
	push ecx
		mov ecx , 0x0
		mov ecx , 0x0
		mov dword [gdtr+2] , ecx

		mov eax ,4*64
		add eax ,ecx
		mov word [gdtr] , ax
		
		lgdt [gdtr]
	pop ecx
	pop eax
	ret
	
load_idt:
	cli
		mov edx, [esp + 4]
		lidt [edx]
	ret


	