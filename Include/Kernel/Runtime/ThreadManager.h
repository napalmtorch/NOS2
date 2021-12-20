#pragma once
#include <Lib/Types.h>
#include <Kernel/Runtime/Thread.h>

namespace Runtime
{
    void ThreadLock();
    void ThreadUnlock();

    namespace ThreadManager
    {
        extern Thread** Threads;
        extern Thread*  CurrentThread;
        extern Thread*  NextThread;

        void Init(Thread* kthread);
        bool Load(Thread* thread);
        bool Unload(Thread* thread);
        bool Terminate(Thread* thread);
        void Ready();
        void Switch();
        void SwitchToKernel();
    }
}