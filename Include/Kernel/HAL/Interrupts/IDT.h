#pragma once
#include <Lib/Types.h>

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10

namespace HAL
{
    typedef enum
    {
        IRQ0 = 32,
        IRQ1,
        IRQ2,
        IRQ3,
        IRQ4,
        IRQ5,
        IRQ6,
        IRQ7,
        IRQ8,
        IRQ9,
        IRQ10,
        IRQ11,
        IRQ12,
        IRQ13,
        IRQ14,
        IRQ15,
    } IRQ;

    typedef struct
    {
        uint32_t DS;
        uint32_t EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX;
        uint32_t INT, ERR;
        uint32_t EIP, CS, EFLAGS, ESP_ORIG, SS;

    } PACKED Registers32;

    typedef struct
    {
        uint16_t BaseLow;
        uint16_t Selector;
        uint8_t  Reserved;
        uint8_t  Flags;
        uint16_t BaseHigh;
    } PACKED IDTEntry;

    typedef struct
    {
        uint16_t Limit;
        uint32_t Base;
    } PACKED IDTR;

    typedef void (*InterruptCallback)(Registers32* regs);

    namespace IDT
    {
        extern InterruptCallback Callbacks[];

        void Init();
        void InitExceptions();
        void InitIRQs();
        void EnableInterrupts();
        void DisableInterrupts();
        void RegisterInterrupt(IRQ num, InterruptCallback callback);
        void UnregisterInterrupt(IRQ num);
        void SetDescriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
    }
}

EXTC
{
    extern void isr0();
    extern void isr1();
    extern void isr2();
    extern void isr3();
    extern void isr4();
    extern void isr5();
    extern void isr6();
    extern void isr7();
    extern void isr8();
    extern void isr9();
    extern void isr10();
    extern void isr11();
    extern void isr12();
    extern void isr13();
    extern void isr14();
    extern void isr15();
    extern void isr16();
    extern void isr17();
    extern void isr18();
    extern void isr19();
    extern void isr20();
    extern void isr21();
    extern void isr22();
    extern void isr23();
    extern void isr24();
    extern void isr25();
    extern void isr26();
    extern void isr27();
    extern void isr28();
    extern void isr29();
    extern void isr30();
    extern void isr31();
    extern void irq0();
    extern void irq1();
    extern void irq2();
    extern void irq3();
    extern void irq4();
    extern void irq5();
    extern void irq6();
    extern void irq7();
    extern void irq8();
    extern void irq9();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();
}
