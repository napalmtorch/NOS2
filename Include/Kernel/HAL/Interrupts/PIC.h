#pragma once
#include <Lib/Types.h>

namespace HAL
{
    namespace PIC
    {
        enum class PICPort
        {
            MasterCMD  = 0x20,
            MasterData = 0x21,
            SlaveCMD   = 0xA0,
            SlaveData  = 0xA1,
        };

        void Remap();
        void Wait(int timeout);
        void WriteMaster(uint8_t value, bool cmd);
        void WriteSlave(uint8_t value, bool cmd);
        void SendEOI(uint8_t num);
    }
}