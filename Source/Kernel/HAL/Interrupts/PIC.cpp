#include <Kernel/HAL/Interrupts/PIC.h>
#include <Kernel/Core/Kernel.h>
using namespace System;

#define PIC_EOI 0x20

namespace HAL
{
    namespace PIC
    {
        void Remap()
        {
            WriteMaster(0x11, true);
            WriteSlave(0x11, true);

            WriteMaster(0x20, false);
            WriteSlave(0x28, false);

            WriteMaster(0x04, false);
            WriteSlave(0x02, false);

            WriteMaster(0x01, false);
            WriteSlave(0x01, false);

            WriteMaster(0x00, false);
            WriteSlave(0x00, false);
            Debug::Info("Re-mapped the PIC");   
        }

        void Wait(int timeout) { while (timeout > 0) { timeout--; } }

        void WriteMaster(uint8_t value, bool cmd)
        {
            if (cmd) { Ports::Write8((uint16_t)PICPort::MasterCMD, value); }
            else { Ports::Write8((uint16_t)PICPort::MasterData, value); }
            Wait(10000);
        }

        void WriteSlave(uint8_t value, bool cmd)
        {
            if (cmd) { Ports::Write8((uint16_t)PICPort::SlaveCMD, value); }
            else { Ports::Write8((uint16_t)PICPort::SlaveData, value); }
            Wait(10000);
        }

        void SendEOI(uint8_t num)
        {
            if (num >= 0x28) { WriteSlave(PIC_EOI, true); }
            WriteMaster(PIC_EOI, true);
            Wait(1000);
        }
    }
}