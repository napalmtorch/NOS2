#include <Lib/Types.h>
#include <Kernel/Core/Kernel.h>

EXTC
{
    void _kernel_entry(void* mboot_ptr)
    {
        Kernel::Boot();
        Kernel::BeforeRun();
        while (true) 
        { 
            Runtime::ThreadLock();
            Kernel::Run(); 
            Runtime::ThreadUnlock();
        }
    }

    extern "C" void __cxa_pure_virtual() { }
}