#pragma once
#include <Lib/Types.h>
#include <Lib/Memory.h>
#include <Lib/Math.h>
#include <Lib/Common.h>
#include <Lib/String.h>
#include <Kernel/HAL/Interrupts/GDT.h>
#include <Kernel/HAL/Interrupts/IDT.h>
#include <Kernel/HAL/Interrupts/PIC.h>
#include <Kernel/HAL/Common/Ports.h>
#include <Kernel/HAL/Common/Serial.h>
#include <Kernel/HAL/Common/PIT.h>
#include <Kernel/HAL/Memory/PMM.h>
#include <Kernel/HAL/Memory/Paging.h>
#include <Kernel/HAL/Drivers/VGA.h>
#include <Kernel/Runtime/ELF.h>
#include <Kernel/Runtime/Thread.h>
#include <Kernel/Runtime/ThreadManager.h>
#include <Kernel/System/Debug.h>

namespace Core
{
    extern HAL::Memory::PhysicalMemoryManager PhysicalMM;
    extern HAL::Drivers::VGAController VGA;
}

namespace Kernel
{
    void Boot();
    void BeforeRun();
    void Run();
    void PITCallback(HAL::Registers32* regs);
}

namespace HAL
{

}

namespace Graphics
{

}