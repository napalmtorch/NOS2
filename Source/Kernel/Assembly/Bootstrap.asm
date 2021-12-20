[BITS 32]
[GLOBAL _mboot]
[GLOBAL _start]
[GLOBAL _stack_top]
[GLOBAL _stack_bottom]
[EXTERN _kernel_entry]

section .multiboot
MBOOT_PAGE_ALIGN    equ 1 << 0
MBOOT_MEM_INFO      equ 1 << 1 
MBOOT_VID_INFO      equ 1 << 2
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

KERNEL_PHYSICAL_BASE equ 0x00100000;
KERNEL_VIRTUAL_BASE  equ 0xC0000000
KERNEL_PAGE_NUMBER   equ (KERNEL_VIRTUAL_BASE >> 22)

_mboot:
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM   
    dd _mboot
    dd 0
    dd 0
    dd 0
    dd _start

section .bss
align 32
_stack_bottom:
    resb 512 * 1024
_stack_top:

section .text
_start:
    cli
    mov EBP, _stack_top         ; set stack base
    mov ESP, EBP                ; set stack pointer
    call _a20_enable            ; enable a20 line
    push EBX                    ; push multiboot address to stack
    call _kernel_entry          ; jump to kernel entry
    cli                         ; disable interrupts
    hlt                         ; halt the system

_a20_enable:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret