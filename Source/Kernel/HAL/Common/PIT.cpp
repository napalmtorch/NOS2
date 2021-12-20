#include <Kernel/HAL/Common/PIT.h>
#include <Kernel/Core/Kernel.h>
using namespace System;

// pit ports
#define PIT_CMD  0x43
#define PIT_DATA 0x40

namespace HAL
{
    namespace PIT
    {
        uint32_t Frequency;
        uint64_t Ticks;
        uint32_t Millis;
        uint64_t MillisTotal;
        uint32_t SecondsTotal;
        double   Timer;

        void Init(uint32_t freq)
        {
             // set frequency variable
            if (freq == 0) { Disable(); }
            if (freq > 5000) { freq = 5000; }
            Frequency = freq;

            // calculate actual frequency
            uint32_t f = 1193180 / Frequency;
            uint8_t high = (uint8_t)((f & 0xFF00) >> 8);
            uint8_t low  = (uint8_t)((f & 0x00FF));

            // send frequency to pit
            Ports::Write8(PIT_CMD, 0x36);
            Ports::Write8(PIT_DATA, low);
            Ports::Write8(PIT_DATA, high);
            
            IDT::RegisterInterrupt(IRQ0, Kernel::PITCallback);
            Debug::OK("Initialized PIT at %d Hz", Frequency);
        }

        void Disable()
        {
            IDT::UnregisterInterrupt(IRQ0);
        }

        void Calculate()
        {
            Ticks++;
            Timer += 1;

            if (Timer >= (double)Frequency / 1000.0)
            {
                Millis++;
                MillisTotal++;
                Timer = 0;
            }

            if (Millis >= 1000)
            {
                SecondsTotal++;
                Millis = 0;
                Timer = 0;
            }
        }

        uint64_t GetTicks() { return Ticks; }

        uint32_t GetFrequency() { return Frequency; }
        
        uint32_t GetMillis() { return Millis; }
        
        uint64_t GetMillisTotal() { return MillisTotal; }
        
        uint32_t GetSecondsTotal() { return SecondsTotal; }
    }
}