#pragma once
#include <Lib/Types.h>

namespace HAL
{
    namespace PIT
    {
        void Init(uint32_t freq);
        void Disable();
        void Calculate();

        uint64_t GetTicks();
        uint32_t GetFrequency();
        uint32_t GetMillis();
        uint64_t GetMillisTotal();
        uint32_t GetSecondsTotal();
    }
}