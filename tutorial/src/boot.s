MBOOT_PAGE_ALIGN	equ 1 << 0	; Load kernel and modules on a page boundary
MBOOT_MEM_INFO		equ 1 << 1	; Provide kernel with memory info
MBOOT_HEADER_MAGIC	equ 0x1badb002	; Multiboot magic value

MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]

[global mboot]		; make accessible from C
[extern code]		; start of .text section
[extern bss]		; start of .bss section
[extern end]		; end of last loadable section

mboot:
	dd MBOOT_HEADER_MAGIC		; GRUB searches for this value
					; on each 4 byte boundary in kernel file
	dd MBOOT_HEADER_FLAGS		; how GRUB should load file / settings
	dd MBOOT_CHECKSUM		; ensure above values are correct

	dd mboot			; location of this descriptor
	dd code				; start of .text
	dd bss				; end of .data / start of .bss
	dd end				; end of kernel
	dd start			; kernel entry point

[global start]				; kernel entry point
[extern main]				; entry point of C code

start:
	push ebx			; load multiboot header location

	cli
	call main
	jmp $
