#include <Kernel/Runtime/Thread.h>
#include <Kernel/Core/Kernel.h>
using namespace HAL;
using namespace System;
using namespace Graphics;

EXTC
{
    extern uint32_t _stack_top;
    extern uint32_t _stack_bottom;
}

namespace Runtime
{
    uint32_t CurrentID;

    Thread* Thread::Create(const char* name, uint32_t stack_size, ThreadProtocol protocol)
    {
        // allocate memory for thread class
        Thread* thread = (Thread*)Core::PhysicalMM.Allocate(sizeof(Thread), true, MemoryState::Thread);
        if (thread == nullptr) { Debug::Error("Unable to create thread"); return nullptr; }

        // initialize and return thread
        thread->Init(name, stack_size, protocol);
        return thread;
    }

    Thread* Thread::CreateELF(const char* name, uint32_t stack_size, uint8_t* data, uint32_t size)
    {
        // allocate memory for thread class
        Thread* thread = (Thread*)Core::PhysicalMM.Allocate(sizeof(Thread), true, MemoryState::Thread);
        if (thread == nullptr) { Debug::Error("Unable to create thread"); return nullptr; }

        // initialize and return thread
        ELFHeader* elf = (ELFHeader*)data;
        thread->InitELF(name, stack_size, data, size);
        return thread;
    }

    Thread* Thread::CreateKernel()
    {
        // allocate memory for thread class
        Thread* thread = (Thread*)Core::PhysicalMM.Allocate(sizeof(Thread), true, MemoryState::Thread);
        if (thread == nullptr) { Debug::Error("Unable to create kernel thread"); return nullptr; }

        // set thread properties
        thread->ID = CurrentID++;
        thread->Stack = (uint32_t*)&_stack_bottom;
        thread->StackSize = &_stack_top - &_stack_bottom;
        thread->Registers.EFLAGS = 0x200;
        thread->State = ThreadState::Running;
        strcpy(thread->Name, "kernel");

        // finished
        Debug::Info("Created kernel thread: ID = 0x%8x, ADDR = 0x%8x, STACK(0x%8x - 0x%8x), NAME = '%s'", 
                        thread->ID, (uint32_t)thread, (uint32_t)thread->Stack, (uint32_t)thread->Stack + thread->StackSize, thread->Name);
        return thread;
    }

    void Thread::Exit(Thread* thread)
    {
        // get exit code from register
        register uint32_t eax asm("eax");
        uint32_t code = eax;

        // set thread state
        ThreadLock();
        IDT::DisableInterrupts();
        thread->ExitCode = code;
        thread->State = ThreadState::Terminated;
        Debug::Info("Thread exited with code %d", code);
        IDT::EnableInterrupts();
        ThreadUnlock();

        // loop forever
        while (true);
    }

    void Thread::Init(const char* name, uint32_t stack_size, ThreadProtocol protocol)
    {
        if (Initialized) { Debug::Warning("Thread %d already initialized", ID); return; }

        // copy name
        for (uint32_t i = 0; i < strlen(name); i++) { if (i < sizeof(Name) - 1) { Name[i] = name[i]; } }
        Name[strlen(Name)] = 0;

        ID          = CurrentID++;
        Stack       = (uint32_t*)Core::PhysicalMM.Allocate(stack_size, true, MemoryState::ThreadStack);
        StackSize   = stack_size;
        Protocol    = protocol;
        State       = ThreadState::Halted;
        Locked      = false;
        Initialized = true;

        // create page table
        Debug::Info("Creating thread page directory");
        PageMgr = Memory::Paging::Create();
        
        // stack start
        uint32_t* s = (uint32_t*)(((uint32_t)Stack + StackSize) - 16);

        // push stack values
        *--s = (uint32_t)this;
        *--s = (uint32_t)0;
        *--s = (uint32_t)Exit;
        *--s = (uint32_t)protocol;

        // set registers
        Registers.ESP    = (uint32_t)s;
        Registers.EBP    = 0x00000000;
        Registers.EFLAGS = 0x00000200;
        Registers.CR3    = PageMgr->GetPhysical();
        Debug::Info("Created thread: ID = 0x%8x, ADDR = 0x%8x, ENTRY = 0x%8x, STACK = 0x%8x, NAME = '%s'", ID, (uint32_t)this, (uint32_t)Protocol, StackSize, Name);
    }

    void Thread::InitELF(const char* name, uint32_t stack_size, uint8_t* data, uint32_t size)
    {
        if (Initialized) { Debug::Warning("Thread %d already initialized", ID); return; }

        // copy name
        for (uint32_t i = 0; i < strlen(name); i++) { if (i < sizeof(Name) - 1) { Name[i] = name[i]; } }
        Name[strlen(Name)] = 0;

        ID          = CurrentID++;
        Stack       = (uint32_t*)Core::PhysicalMM.Allocate(stack_size, true, MemoryState::ThreadStack);
        StackSize   = stack_size;
        State       = ThreadState::Halted;
        Locked      = false;
        Initialized = true;

        // create page table
        Debug::Info("Creating thread page directory");
        PageMgr = Memory::Paging::Create();

        ELFHeader* elf = (ELFHeader*)data;
        ELF::Start(data, size, this);
        Protocol = (ThreadProtocol)elf->Entry;
        
        // stack start
        uint32_t* s = (uint32_t*)(((uint32_t)Stack + StackSize) - 16);

        // push stack values
        *--s = (uint32_t)this;
        *--s = (uint32_t)0;
        *--s = (uint32_t)Exit;
        *--s = (uint32_t)Protocol;

        // set registers
        Registers.ESP    = (uint32_t)s;
        Registers.EBP    = 0x00000000;
        Registers.EFLAGS = 0x00000200;
        Registers.CR3    = PageMgr->GetPhysical();
        Debug::Info("Created ELF thread: ID = 0x%8x, ADDR = 0x%8x, ENTRY = 0x%8x, STACK = 0x%8x, NAME = '%s'", ID, (uint32_t)this, (uint32_t)Protocol, StackSize, Name);
    }

    bool Thread::Start()
    {
        return false;
    }

    bool Thread::Stop()
    {
        return false;
    }

    bool Thread::Lock() { Locked = true; }

    bool Thread::Unlock() { Locked = false; }
}