#include <Kernel/HAL/Interrupts/IDT.h>
#include <Kernel/Core/Kernel.h>
using namespace System;

#define IDT_COUNT 256
#define IDT_FLAG  0x8E

EXTC
{
    extern void _idt_flush(uint32_t v);
    extern void _reload_segments();

     uint32_t ESP, EBP, ESI, EDI, EAX, EBX, ECX, EDX;
        uint32_t EFLAGS, DS, CR3;

    void _isr_handler(HAL::Registers32* regs)
    {
        HAL::PIC::SendEOI(regs->INT);
        Debug::Error("ISR HANDLER: INT = 0x%2x, ERR = 0x%2x", regs->INT, regs->ERR);
        if (Runtime::ThreadManager::CurrentThread != nullptr)
        {
            Debug::Error("THREAD = '%s'", Runtime::ThreadManager::CurrentThread->Name);

            Runtime::ThreadRegisters r = Runtime::ThreadManager::CurrentThread->Registers;
            Debug::PrintFormatted("EAX: 0x%8x EBX: 0x%8x ECX: 0x%8x EDX: 0x%8x\n", r.EAX, r.EBX, r.ECX, r.EDX);
            Debug::PrintFormatted("ESI: 0x%8x EDI: 0x%8x ESP: 0x%8x EBP: 0x%8x\n", r.ESI, r.EDI, r.ESP, r.EBP);
            Debug::PrintFormatted("EFL: 0x%8x CR3: 0x%8x DS:  0x%4x\n", r.EFLAGS, r.CR3, r.DS);

            if (Runtime::ThreadManager::CurrentThread->ID > 0)
            {
                Runtime::ThreadManager::Terminate(Runtime::ThreadManager::CurrentThread);
                Runtime::ThreadManager::SwitchToKernel();
            }
            else { Debug::Error("Kernel panic"); asm volatile("cli; hlt"); }
        }
    }

    void _irq_handler(HAL::Registers32* regs)
    {
        HAL::PIC::SendEOI(regs->INT);
        if (HAL::IDT::Callbacks[regs->INT] != nullptr) { HAL::IDT::Callbacks[regs->INT](regs); }
    }
}

namespace HAL
{
    namespace IDT
    {
        InterruptCallback Callbacks[IDT_COUNT];
        IDTEntry          Entries[IDT_COUNT];
        IDTR              Pointer;

        void Init()
        {
            Debug::Info("Setting up IDT entries");
            memset(Callbacks, 0, IDT_COUNT * sizeof(InterruptCallback));
            memset(Entries, 0, IDT_COUNT);

            Pointer.Limit = (sizeof(IDTEntry) * IDT_COUNT) - 1;
            Pointer.Base  = (uint32_t)&Entries;

            InitExceptions();
            PIC::Remap();
            InitIRQs();

            _idt_flush((uint32_t)&Pointer);
            Debug::OK("Initialized IDT");
        }

        void InitExceptions()
        {
            SetDescriptor(0,  (uint32_t)isr0,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(1,  (uint32_t)isr1,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(2,  (uint32_t)isr2,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(3,  (uint32_t)isr3,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(4,  (uint32_t)isr4,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(5,  (uint32_t)isr5,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(6,  (uint32_t)isr6,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(7,  (uint32_t)isr7,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(8,  (uint32_t)isr8,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(9,  (uint32_t)isr9,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(10, (uint32_t)isr10, KERNEL_CS, IDT_FLAG);
            SetDescriptor(11, (uint32_t)isr11, KERNEL_CS, IDT_FLAG);
            SetDescriptor(12, (uint32_t)isr12, KERNEL_CS, IDT_FLAG);
            SetDescriptor(13, (uint32_t)isr13, KERNEL_CS, IDT_FLAG);
            SetDescriptor(14, (uint32_t)isr14, KERNEL_CS, IDT_FLAG);
            SetDescriptor(15, (uint32_t)isr15, KERNEL_CS, IDT_FLAG);
            SetDescriptor(16, (uint32_t)isr16, KERNEL_CS, IDT_FLAG);
            SetDescriptor(17, (uint32_t)isr17, KERNEL_CS, IDT_FLAG);
            SetDescriptor(18, (uint32_t)isr18, KERNEL_CS, IDT_FLAG);
            SetDescriptor(19, (uint32_t)isr19, KERNEL_CS, IDT_FLAG);
            SetDescriptor(20, (uint32_t)isr20, KERNEL_CS, IDT_FLAG);
            SetDescriptor(21, (uint32_t)isr21, KERNEL_CS, IDT_FLAG);
            SetDescriptor(22, (uint32_t)isr22, KERNEL_CS, IDT_FLAG);
            SetDescriptor(23, (uint32_t)isr23, KERNEL_CS, IDT_FLAG);
            SetDescriptor(24, (uint32_t)isr24, KERNEL_CS, IDT_FLAG);
            SetDescriptor(25, (uint32_t)isr25, KERNEL_CS, IDT_FLAG);
            SetDescriptor(26, (uint32_t)isr26, KERNEL_CS, IDT_FLAG);
            SetDescriptor(27, (uint32_t)isr27, KERNEL_CS, IDT_FLAG);
            SetDescriptor(28, (uint32_t)isr28, KERNEL_CS, IDT_FLAG);
            SetDescriptor(29, (uint32_t)isr29, KERNEL_CS, IDT_FLAG);
            SetDescriptor(30, (uint32_t)isr30, KERNEL_CS, IDT_FLAG);
            SetDescriptor(31, (uint32_t)isr31, KERNEL_CS, IDT_FLAG);
            Debug::OK("Initialized exception handlers");
        }

        void InitIRQs()
        {
            SetDescriptor(32, (uint32_t)irq0,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(33, (uint32_t)irq1,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(34, (uint32_t)irq2,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(35, (uint32_t)irq3,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(36, (uint32_t)irq4,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(37, (uint32_t)irq5,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(38, (uint32_t)irq6,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(39, (uint32_t)irq7,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(40, (uint32_t)irq8,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(41, (uint32_t)irq9,  KERNEL_CS, IDT_FLAG);
            SetDescriptor(42, (uint32_t)irq10, KERNEL_CS, IDT_FLAG);
            SetDescriptor(43, (uint32_t)irq11, KERNEL_CS, IDT_FLAG);
            SetDescriptor(44, (uint32_t)irq12, KERNEL_CS, IDT_FLAG);
            SetDescriptor(45, (uint32_t)irq13, KERNEL_CS, IDT_FLAG);
            SetDescriptor(46, (uint32_t)irq14, KERNEL_CS, IDT_FLAG);
            SetDescriptor(47, (uint32_t)irq15, KERNEL_CS, IDT_FLAG);
            Debug::OK("Initialized IRQ handlers");
        }

        void EnableInterrupts() { asm volatile("sti"); }
        void DisableInterrupts() { asm volatile("cli"); }

        void RegisterInterrupt(IRQ num, InterruptCallback callback)
        {
            Callbacks[num] = callback;
            Debug::Info("Registered interrupt: NUM = 0x%2x, CALLBACK = 0x%8x", num, callback);
        }

        void UnregisterInterrupt(IRQ num)
        {
            InterruptCallback callback = Callbacks[num];
            Callbacks[num] = nullptr;
            Debug::Info("Unregistered interrupt: NUM = 0x%2x, CALLBACK = 0x%8x", num, callback);
        }

        void SetDescriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
        {
            Entries[num].BaseLow  = base & 0xFFFF;
            Entries[num].BaseHigh = (base >> 16) & 0xFFFF;
            Entries[num].Selector = sel;
            Entries[num].Flags    = flags;
            Entries[num].Reserved = 0;
        }
    }
}