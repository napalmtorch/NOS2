#include <Kernel/HAL/Common/Ports.h>

namespace HAL
{
    namespace Ports
    {
        uint8_t Read8(uint16_t port)
        {
            uint8_t result;
            asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
            return result;
        } 

        uint16_t Read16(uint16_t port)
        {
            uint16_t result;
            asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
            return result;
        }

        uint32_t Read32(uint16_t port)
        {
            uint32_t result;
            asm volatile("inl %%dx, %%eax" : "=a" (result) : "dN" (port));
            return result;
        }

        void Write8(uint16_t port, uint8_t data) { asm volatile("out %%al, %%dx" : : "a" (data), "d" (port)); }

        void Write16(uint16_t port, uint16_t data) { asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port)); }

        void Write32(uint16_t port, uint32_t data) { asm volatile("outl %%eax, %%dx" : : "dN" (port), "a" (data)); }
    }
}