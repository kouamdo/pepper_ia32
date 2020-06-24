global load_idt , load_gdt
extern __idt__

gdtr DW 0	;For limit
	DD 0	;For Base

section .text

load_gdt:
	cli
	push eax
	push ecx
		mov ecx , 0x0
		mov dword [gdtr+2] , ecx

		xor eax , eax
		mov eax ,4*64
		add eax ,ecx
		mov word [gdtr] , ax
		
		lgdt [gdtr]
	pop ecx
	pop eax
	ret

idtr dw 0
	dd 0

load_idt:
	cli

	push eax
	push ecx
		xor ecx ,ecx
		mov ecx , __idt__
		mov dword [idtr+2] , ecx

		xor eax , eax
		mov eax ,16*64
		add eax ,ecx
		mov word [idtr] , ax
		
		lidt [idtr]
	pop ecx
	pop eax
	ret


	