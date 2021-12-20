#include <Kernel/HAL/Memory/PMM.h>
#include <Kernel/Core/Kernel.h>
using namespace System;

#define MM_ALIGN 0x1000

namespace HAL
{
    namespace Memory
    {
        void PhysicalMemoryManager::Init(uint32_t start, uint32_t max_entries, uint32_t pages)
        {
            if (Initialized) { return; }

            // clear info table
            memset(&Info, 0, sizeof(PhysicalMemoryInfo));

            // determine table size
            uint32_t table_size = AlignValue(max_entries * sizeof(PhysicalMemoryEntry));
            if (table_size < MM_ALIGN) { table_size = MM_ALIGN; }

            // setup table information
            Info.TableStart    = start;
            Info.TableSize     = table_size;
            Info.TableCountMax = table_size / sizeof(PhysicalMemoryEntry);
            Info.TableCount    = 0;
            Entries            = (PhysicalMemoryEntry*)Info.TableStart;
            Debug::Info("PMM Table: ADDR = 0x%8x, MAX = %d, SIZE = %d bytes", Info.TableStart, Info.TableCountMax, Info.TableSize);

            // setup data information
            Info.DataStart = Info.TableStart + Info.TableSize;
            Info.DataSize  = pages * (4 * 1024 * 1024);
            Info.DataUsed  = 0;
            Debug::Info("PMM Data:  ADDR = 0x%8x, SIZE = %d bytes", Info.DataStart, Info.DataSize);

            // map
            uint32_t addr = Info.TableStart;
            uint32_t maps = (GetTotalSize() / (4 * 1024 * 1024)) + 1;
            uint32_t end  = Info.TableStart + (maps * (4 * 1024 * 1024));
            for (uint32_t i = 0; i < maps; i++) { Paging::KernelDirectory.Map(addr, addr, false); addr += 4 * 1024 * 1024; }
            memset((void*)Info.TableStart, 0, table_size);
            memset((void*)Info.DataStart, 0, Info.DataSize);
            Debug::Info("Identity mapped 0x%8x(ID = %d) - 0x%8x(ID = %d)", Info.TableStart, Info.TableStart >> 22, end, end >> 22);

            // create mass free entry
            MassEntry = CreateEntry(Info.DataStart, Info.DataSize, MemoryState::Free);

            // finished
            Initialized = true;
            Debug::OK("Initialized physical memory manager(0x%8x - 0x%8x), size = %d(+4) MB", Info.TableStart, end, Info.DataSize / 1024 / 1024);
        }

        void* PhysicalMemoryManager::Allocate(uint32_t size, bool clear, MemoryState state)
        {
            if (!Initialized) { return nullptr; }
            PhysicalMemoryEntry* entry = GetNextEntry(AlignValue(size));
            if (entry == nullptr) { Debug::Error("Unable to locate free entry while allocating %d bytes", size); return nullptr; }

            if (state != MemoryState::Free) { entry->State = state; }
            if (clear) { memset(entry->Pointer, 0, entry->Size); }
            Info.DataUsed += size;

            Debug::Header("MALLOC", Graphics::Color4::Green, Debug::GetMode());
            Debug::PrintFormatted("ADDR = 0x%8x, STATE = 0x%2x, SIZE = %d bytes\n", (uint32_t)entry->Pointer, (uint32_t)entry->State, entry->Size);
            return entry->Pointer;

        }

        void* PhysicalMemoryManager::Allocate(uint32_t size, bool clear) { return Allocate(size, clear, MemoryState::Used); }

        void PhysicalMemoryManager::Free(void* ptr)
        {
            if (ptr == nullptr) { return; }

            for (uint32_t i = 0; i < Info.TableCountMax; i++)
            {
                if (Entries[i].Pointer == ptr)
                {
                    MemoryState state = Entries[i].State;
                    Entries[i].State = MemoryState::Free;
                    memset(Entries[i].Pointer, 0, Entries[i].Size);
                    Debug::Header(" FREE ", Graphics::Color4::Yellow, Debug::GetMode());
                    Debug::PrintFormatted("ADDR = 0x%8x, STATE = 0x%2x, SIZE = %d bytes\n", (uint32_t)Entries[i].Pointer, (uint32_t)state, Entries[i].Size);
                    return;
                }
            }

            Debug::Error("Unable to free pointer 0x%8x", (uint32_t)ptr);
        }

        PhysicalMemoryEntry* PhysicalMemoryManager::GetNextEntry(uint32_t size)
        {
            if (size == 0) { Debug::Error("Tried to get next entry with size 0"); return nullptr; }
            if (!IsValueAligned(size)) { Debug::Error("Tried to get next entry with un-aligned size %d", size); return nullptr; }

            for (uint32_t i = 0; i < Info.TableCountMax; i++)
            {
                if (IsNullAtIndex(i)) { continue; }
                if (IsAddressValid((uint32_t)Entries[i].Pointer) && Entries[i].Size == size && Entries[i].State == MemoryState::Free)
                {
                    Entries[i].State = MemoryState::Used;
                    return &Entries[i];
                }
            }

            uint32_t addr = (uint32_t)MassEntry->Pointer;

            MassEntry->Pointer =  (void*)((uint32_t)MassEntry->Pointer + size);
            MassEntry->Size    -= size;
            MassEntry->State   =  MemoryState::Free;

            PhysicalMemoryEntry* entry = CreateEntry(addr, size, MemoryState::Used);
            return entry;
        }

        PhysicalMemoryEntry* PhysicalMemoryManager::CreateEntry(uint32_t addr, uint32_t size, MemoryState state)
        {
            if (addr == 0)             { Debug::Error("Tried to create PMM entry with address of 0"); return nullptr; }
            if (size == 0)             { Debug::Error("Tried to create PMM entry with size of 0"); return nullptr; }
            if (!IsValueAligned(addr)) { Debug::Error("Tried to create PMM entry with un-aligned address 0x%8x", addr); return nullptr; }
            if (!IsValueAligned(size)) { Debug::Error("Tried to create PMM entry with un-aligned size %d", size); return nullptr; }

            int index = GetFreeIndex();
            if (index < 0 || index >= Info.TableCountMax) { Debug::Error("Unable to get free PMM entry index"); return nullptr; }

            Entries[index].Pointer = (void*)addr;
            Entries[index].Size    = size;
            Entries[index].State   = state;
            
            Info.TableCount++;
            //Debug::Info("Created PMM entry: ID = 0x%8x, ADDR = 0x%8x, STATE = 0x%2x, SIZE = %d bytes", index, addr, (uint8_t)state, size);
            return &Entries[index];
        }

        bool PhysicalMemoryManager::DeleteEntry(PhysicalMemoryEntry* entry)
        {
            if (entry == nullptr) { Debug::Error("Tried to delete nullptr PMM entry"); return false; }
            for (uint32_t i = 0; i < Info.TableCountMax; i++)
            {
                if (Entries[i].Pointer == nullptr) { continue; }
                if (&Entries[i] == entry)
                {
                    uint32_t addr, size, state;
                    addr  = (uint32_t)Entries[i].Pointer;
                    size  = Entries[i].Size;
                    state = (uint32_t)Entries[i].State;

                    Entries[i].Pointer = nullptr;
                    Entries[i].Size    = 0;
                    Entries[i].State   = MemoryState::Free;
                    //Debug::Info("Deleted PMM entry: ID = 0x%8x, ADDR = 0x%8x, STATE = 0x%2x, SIZE = %d bytes", i, addr, state, size);
                    return true;
                }
            }

            Debug::Error("Unable to delete PMM entry 0x%8x", (uint32_t)entry);
        }

        int PhysicalMemoryManager::GetFreeIndex()
        {
            for (uint32_t i = 0; i < Info.TableCountMax; i++)
            {
                if (IsNullAtIndex(i))  { return i; }
            }
            return -1;
        }

        uint32_t PhysicalMemoryManager::AlignValue(uint32_t val)
        {
            uint32_t out = val;
            out &= 0xFFFFF000;
            if (out < val) { out += MM_ALIGN; }
            return out;
        }

        PhysicalMemoryInfo PhysicalMemoryManager::GetInfo() { return Info; }

        uint32_t PhysicalMemoryManager::GetTotalSize() { return Info.TableSize + Info.DataSize; }

        bool PhysicalMemoryManager::IsAddressValid(uint32_t addr)
        {
            if (addr >= Info.DataStart && addr < Info.DataStart + Info.DataSize) { return true; }
            return false;
        }

        bool PhysicalMemoryManager::IsValueAligned(uint32_t val)
        {
            return (uint32_t)(val % MM_ALIGN) == 0;
        }

        bool PhysicalMemoryManager::IsNullAtIndex(int index)
        {
            if (Entries[index].Pointer == nullptr && Entries[index].Size == 0 && Entries[index].State == MemoryState::Free) { return true; }
            return false;
        }

        bool PhysicalMemoryManager::IsInitialized() { return Initialized; }
    }
}