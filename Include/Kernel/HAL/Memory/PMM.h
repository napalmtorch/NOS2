#pragma once
#include <Lib/Types.h>

namespace HAL
{
    enum class MemoryState : uint16_t
    {
        Free,
        Used,
        String,
        Bitmap,
        Array,
        PointerArray,
        Thread,
        ThreadStack,
        PageDirectory,
    };

    namespace Memory
    {
        typedef struct
        {
            void*        Pointer;
            uint32_t     Size;
            MemoryState  State;
        } PACKED PhysicalMemoryEntry;

        typedef struct
        {
            uint32_t TableStart;
            uint32_t TableSize;
            uint32_t TableCount;
            uint32_t TableCountMax;
            uint32_t DataStart;
            uint32_t DataSize;
            uint32_t DataUsed;
        } PhysicalMemoryInfo;

        class PhysicalMemoryManager
        {
            private:
                PhysicalMemoryInfo   Info;
                PhysicalMemoryEntry* Entries;
                PhysicalMemoryEntry* MassEntry;
                bool                 Initialized;

            public:
                void  Init(uint32_t start, uint32_t max_entries, uint32_t pages);
                void* Allocate(uint32_t size, bool clear, MemoryState state);
                void* Allocate(uint32_t size, bool clear);
                void  Free(void* ptr);

            private:
                PhysicalMemoryEntry* GetNextEntry(uint32_t size);
                PhysicalMemoryEntry* CreateEntry(uint32_t addr, uint32_t size, MemoryState state);
                bool DeleteEntry(PhysicalMemoryEntry* entry);
                int GetFreeIndex();

            public:
                uint32_t AlignValue(uint32_t val);
                PhysicalMemoryInfo GetInfo();
                uint32_t GetTotalSize();
                bool IsAddressValid(uint32_t addr);
                bool IsValueAligned(uint32_t val);
                bool IsNullAtIndex(int index);
                bool IsInitialized();
        };
    }
}