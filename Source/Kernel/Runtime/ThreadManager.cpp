#include <Kernel/Runtime/ThreadManager.h>
#include <Kernel/Core/Kernel.h>
using namespace HAL;
using namespace System;
using namespace Graphics;

#define THREAD_MAXCOUNT 4096

EXTC
{
    extern void _switch_threads();
}

namespace Runtime
{   
    namespace ThreadManager
    {
        Thread** Threads;
        Thread*  CurrentThread;
        Thread*  NextThread;
        uint32_t Count, Index;
        bool     Initialized;
        bool     CanSwitch;

        int GetFreeIndex();

        void Init(Thread* kthread)
        {
            Threads = (Thread**)Core::PhysicalMM.Allocate(sizeof(Thread*) * THREAD_MAXCOUNT, true, MemoryState::PointerArray);

            Threads[0] = kthread;
            CurrentThread    = kthread;
            NextThread       = nullptr;
            Index            = 0;
            Count            = 1;
            CanSwitch        = false;
            Initialized      = true;

            Debug::OK("Initialized thread manager");
        }

        bool Load(Thread* thread)
        {
            if (thread == nullptr) { Debug::Error("Tried to load null thread"); return false; }

            IDT::DisableInterrupts();

            int i = GetFreeIndex();
            if (i < 0 || i >= THREAD_MAXCOUNT) { Debug::Error("Maximum amount of threads reached"); return false; }

            Count++;
            Threads[i] = thread;
            Threads[i]->State = ThreadState::Running;

            Debug::Info("Loaded thread: ID = %d, NAME = '%s'", thread->ID, thread->Name);
            IDT::EnableInterrupts();
            return true;
        }

        bool Unload(Thread* thread)
        {
            IDT::DisableInterrupts();

            IDT::EnableInterrupts();
            return false;
        }

        bool Terminate(Thread* thread)
        {
            IDT::DisableInterrupts();
            for (uint32_t i = 0; i < THREAD_MAXCOUNT; i++)
            {
                if (Threads[i] == nullptr) { continue; }
                if (Threads[i] == thread)
                {
                    Threads[i]->State = ThreadState::Terminated;
                    IDT::EnableInterrupts();
                    return true;
                }
            }
            IDT::EnableInterrupts();
            return false;
        }

        void Ready() 
        { 
            if (!Initialized) { return; }
            CanSwitch = true; 
        }

        void Switch()
        {
            if (!CanSwitch) { return; }

            IDT::DisableInterrupts();

            // get current thread
            CurrentThread = Threads[Index];
            if (CurrentThread == nullptr) { IDT::EnableInterrupts(); Index = 0; return; }
            if (CurrentThread->Locked) { IDT::EnableInterrupts(); return; }

            // get next thread
            Index++;
            while (Threads[Index] == nullptr) { Index++; }
            if (Index >= THREAD_MAXCOUNT) { Index = 0; }
            NextThread = Threads[Index];

            if (NextThread->State == ThreadState::Terminated)
            {
                Debug::Info("Terminated thread '%s'", NextThread->Name);
                Core::PhysicalMM.Free(NextThread->Stack);
                Core::PhysicalMM.Free(NextThread);
                Threads[Index] = nullptr;
                NextThread = Threads[0];
                Index = 0;
            }

            _switch_threads();
        }

        void SwitchToKernel()
        {
            Index = 0;
            NextThread = Threads[Index];
            _switch_threads();
        }

        int GetFreeIndex()
        {
            for (uint32_t i = 0; i < THREAD_MAXCOUNT; i++)
            {
                if (Threads[i] == nullptr) { return i; }
            }
            return -1;
        }
    }

    void ThreadLock() 
    { 
        if (ThreadManager::CurrentThread == nullptr) { return; }
        ThreadManager::CurrentThread->Lock(); 
    }
    void ThreadUnlock()
    {
        if (ThreadManager::CurrentThread == nullptr) { return; }
        ThreadManager::CurrentThread->Unlock();
    }
}