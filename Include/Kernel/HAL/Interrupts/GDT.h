#pragma once
#include <Lib/Types.h>

namespace HAL
{
    namespace GDT
    {
        void     Init();
        int      SetDescriptor();
        int      AddDescriptor(uint8_t id, uint64_t desc);
        uint64_t CreateDescriptor(uint32_t base, uint32_t limit, uint16_t flag);
    }
}