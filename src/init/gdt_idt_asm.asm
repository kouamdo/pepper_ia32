global load_idt , load_gdt

section .text

load_gdt:
	cli
	mov edx , [esp+4]
	lgdt [edx]
	ret
	
load_idt:
	cli
		mov edx, [esp + 4]
		lidt [edx]
	ret

