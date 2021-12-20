#pragma once
#include <Lib/Types.h>
#include <Kernel/HAL/Interrupts/IDT.h>
#include <Kernel/HAL/Memory/Paging.h>

namespace Runtime
{
    typedef int (*ThreadProtocol)(void* arg);

    typedef struct
    {
        uint32_t ESP, EBP, ESI, EDI, EAX, EBX, ECX, EDX;
        uint32_t EFLAGS, DS, CR3;
    } ThreadRegisters;

    enum class ThreadState : uint8_t
    {
        Halted,
        Running,
        Terminated,
    };

    class Thread
    {
        public:
            ThreadRegisters Registers;
            uint32_t*       Stack;
            uint32_t        StackSize;
            ThreadState     State;
            ThreadProtocol  Protocol;
            uint32_t        ID;
            uint32_t        ExitCode;
            bool            Locked;
            bool            Initialized;
            uint32_t        ELFVirtual;
            uint32_t        ELFPhysical;
            char            Name[140];

        public:
            HAL::Memory::PageManager* PageMgr;

        public:
            static Thread* Create(const char* name, uint32_t stack_size, ThreadProtocol protocol);
            static Thread* CreateELF(const char* name, uint32_t stack_size, uint8_t* data, uint32_t size);
            static Thread* CreateKernel();
            static void    Exit(Thread* thread);

        public:
            void Init(const char* name, uint32_t stack_size, ThreadProtocol protocol);
            void InitELF(const char* name, uint32_t stack_size, uint8_t* data, uint32_t size);
            bool Start();
            bool Stop();
            bool Lock();
            bool Unlock();
    };
}