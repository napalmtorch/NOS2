[GLOBAL _switch_threads]
[EXTERN _ZN7Runtime13ThreadManager13CurrentThreadE]
[EXTERN _ZN7Runtime13ThreadManager10NextThreadE]

;ESP, EBP, ESI, EDI, EAX, EBX, ECX, EDX;
;EFLAGS, DS, CR3;

_switch_threads:
    ; load current thread into register
    mov EAX, [_ZN7Runtime13ThreadManager13CurrentThreadE]  

    ; save general purpose registers
    mov [EAX+0],  ESP
    mov [EAX+4],  EBP
    mov [EAX+8],  ESI
    mov [EAX+12], EDI
    mov [EAX+16], EAX
    mov [EAX+20], EBX
    mov [EAX+24], ECX
    mov [EAX+28], EDX
    
    ; save ds
    ;mov CX, DS
    ;mov [EAX+36], ECX

    ; save cr3
    mov ECX, CR3
    mov [EAX+40], ECX
    xchg bx, bx

    ; save eflags
    pushf
    pop ECX
    mov [EAX+32], ECX

    ; load next thread into register
    mov EAX, [_ZN7Runtime13ThreadManager10NextThreadE]
    
    ; load ds
    ;mov ECX, [EAX+36]
    ;mov DS, CX
    
    ; load cr3
    mov ECX, [EAX+40]
    mov CR3, ECX
    
    ; load eflags
    mov ECX, [EAX+32]
    push ECX
    popf

    ; load general purpose registers
    mov ESP, [EAX+0]
    mov EBP, [EAX+4]
    mov ESI, [EAX+8]
    mov EDI, [EAX+12]
    mov EAX, [EAX+16]
    mov EBX, [EAX+20]
    mov ECX, [EAX+24]
    mov EDX, [EAX+28]
    
    ; re-enable interrupts and return to address on next stack
    sti
    ret

