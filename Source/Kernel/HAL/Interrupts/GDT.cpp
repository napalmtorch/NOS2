#include <Kernel/HAL/Interrupts/GDT.h>
#include <Kernel/Core/Kernel.h>
using namespace System;

#define GDT_ACCESSED	0x01
#define GDT_READWRITE	0x02
#define GDT_DIRECTION	0x04
#define GDT_EXEC	    0x08
#define GDT_STATIC	    0x10
#define GDT_PRESENT	    0x20

EXTC
{
    extern void _gdt_flush();
    extern void _reload_segments();
}

namespace HAL
{
    namespace GDT
    {
        static uint32_t GDTPointer = 0;
        static uint32_t GDTSize    = 0;
        static uint32_t GDTRLocation    = 0;

        static uint32_t HighPart;
        static uint32_t LowPart;

        void Init()
        {
            GDTPointer   = 0x806;
            GDTRLocation = 0x800;

            AddDescriptor(0, 0);
            AddDescriptor(1, 0x00CF9A000000FFFF);
            AddDescriptor(2, 0x00CF92000000FFFF);
            AddDescriptor(3, 0x008FFA000000FFFF);
            SetDescriptor();
        }

        int SetDescriptor()
        {
            *(uint16_t*)GDTRLocation = (GDTSize - 1) & 0x0000FFFF;
            GDTRLocation += 2;
            *(uint32_t*)GDTRLocation = GDTPointer;
            _gdt_flush();
            return 0;
        }

        int AddDescriptor(uint8_t id, uint64_t desc)
        {
            uint32_t loc = GDTPointer + sizeof(uint64_t)*id;
            *(uint64_t*)loc = desc;
            GDTSize += sizeof(desc);
            Debug::Info("Added GDT descriptor(ID = 0x%2x, BASE = 0x%8x%8x", id, (uint32_t)((desc & 0xFFFFFFFF00000000) >> 32), (uint32_t)(desc & 0xFFFFFFFF));
            return 0;
        }

        uint64_t CreateDescriptor(uint32_t base, uint32_t limit, uint16_t flag)
        {
            uint64_t desc = 0;
            HighPart = 0;
            LowPart  = 0;
            desc = limit         & 0x000F0000;
            desc |= (flag << 8)  & 0x00F0FF00;
            desc |= (base >> 16) & 0x000000FF;
            desc |= base         & 0xFF000000;
            
            HighPart = desc;
            desc <<= 32;

            desc |= base << 16;
            desc |= limit & 0x0000FFFF;
            LowPart = (uint32_t)desc;
	        return desc;
        }
    }
}