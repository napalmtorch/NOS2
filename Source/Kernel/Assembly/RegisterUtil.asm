[GLOBAL _read_cr0]
[GLOBAL _read_cr3]
[GLOBAL _write_cr0]
[GLOBAL _write_cr3]
[GLOBAL _reload_segments]

_read_cr0:
    mov EAX, CR0
    retn

_read_cr3:
    mov EAX, CR3
    retn

_write_cr0:
    push EBP
    mov  EBP, ESP
    mov  EAX, [EBP+8]
    mov  CR0, EAX
    pop  EBP
    retn

_write_cr3:
    push EBP
    mov  EBP, ESP
    mov  EAX, [EBP+8]
    mov  CR3, EAX
    pop  EBP
    retn

_reload_segments:
    push EBP
    mov EBP, ESP

    push EAX
    mov AX, 0x10
    mov DS, AX
    mov ES, AX
    mov FS, AX
    mov GS, AX
    mov SS, AX
    pop EAX

    jmp 0x8:_reload_segments_end
_reload_segments_end:
    mov ESP, EBP
    pop EBP
    ret