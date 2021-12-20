#include <Kernel/HAL/Memory/Paging.h>
#include <Kernel/Core/Kernel.h>
using namespace System;

EXTC
{
    extern uint32_t _read_cr0();
    extern uint32_t _read_cr3();
    extern void     _write_cr0(uint32_t cr0);
    extern void     _write_cr3(uint32_t cr3);
}

#define KERNEL_DIR 0x00400000
#define PAGE_COUNT 1024

namespace HAL
{
    namespace Memory
    {
        namespace Paging
        {
            PageManager KernelDirectory;

            void Init()
            {
                KernelDirectory.Init(KERNEL_DIR);
                KernelDirectory.Map(0, 0);
                KernelDirectory.Map(KernelDirectory.GetPhysical(), KernelDirectory.GetPhysical());
                Debug::OK("Initialized kernel page manager at 0x%8x", KernelDirectory.GetPhysical());

                _write_cr3(KernelDirectory.GetPhysical());
                _write_cr0(_read_cr0() | 0x80000000);
                Debug::OK("Initialized paging");
            }

            PageManager* Create()
            {
                PageManager* pm = (PageManager*)Core::PhysicalMM.Allocate(sizeof(PageManager), false);
                uint8_t* pm_phys = (uint8_t*)0x00800000; //Core::PhysicalMM.Allocate((PAGE_COUNT * 4) + (PAGE_COUNT * 4), false, MemoryState::PageDirectory);
                KernelDirectory.Map((uint32_t)pm_phys, (uint32_t)pm_phys);
                pm->Init((uint32_t)pm_phys);
                pm->Map(0, 0);
                pm->Map(Paging::KernelDirectory.GetPhysical(), Paging::KernelDirectory.GetPhysical());
                pm->Map((uint32_t)pm_phys, (uint32_t)pm_phys);
                pm->Map(Core::PhysicalMM.GetInfo().TableStart, Core::PhysicalMM.GetInfo().TableStart);
                

                Debug::OK("Initialized page manager at 0x%8x", (uint32_t)pm_phys);
                return pm;
            }
        }

        void PageManager::Init(uint32_t pd_start)
        {
            Physical = pd_start;
            for (uint32_t i = 0; i < PAGE_COUNT; i++) { WriteEntry(i, 0 | 2); }
        }

        void PageManager::Map(uint32_t v_addr, uint32_t p_addr, bool msg)
        {
            uint32_t   start = p_addr;
            uint32_t   id    = v_addr >> 22;
            PageTable* table = GetFreePageTable();
            uint32_t   index = GetPageTableIndex(table); 

            if (table == nullptr) { Debug::Error("Unable to map page table"); return; }
            
            table->Entries  = (uint32_t*)((Physical + (PAGE_COUNT * 4)) + (index * (PAGE_COUNT * 4)));
            table->Physical = p_addr;
            table->Virtual  = v_addr;
            table->Used     = true;

            for (uint32_t i = 0; i < PAGE_COUNT; i++) { table->Entries[i] = p_addr | 3; p_addr += 0x1000; }
            WriteEntry(id, (uint32_t)table->Entries | 3);
            if (msg) { Debug::Info("Mapped virtual address 0x%8x(ID = %d) to physical address 0x%8x", v_addr, id, start); }
        }

        void PageManager::Unmap(void* v_addr)
        {
            uint32_t id = (uint32_t)v_addr >> 22;

            for (uint32_t i = 0; i < PAGE_COUNT; i++)
            {
                if (Tables[i].Used && Tables[i].Virtual == (uint32_t)v_addr)
                {
                    Tables[i].Physical = 0;
                    Tables[i].Virtual  = 0;
                    Tables[i].Used     = false;

                    WriteEntry(id, 0 | 2);
                    Debug::Info("Un-mapped virtual address 0x%8x(ID = %d)", v_addr, id);
                    return;
                }
            }

            Debug::Error("Unable to un-map page table at virtual address 0x%8x(ID = %d)", v_addr, id);
        }

        void PageManager::WriteEntry(int index, uint32_t table)
        {
            uint32_t* entries = (uint32_t*)Physical;
            entries[index] = table;
        }

        uint32_t PageManager::ReadEntry(int index)
        {
            uint32_t* entries = (uint32_t*)Physical;
            return entries[index];
        }

        PageTable* PageManager::GetFreePageTable()
        {
            for (uint32_t i = 0; i < PAGE_COUNT; i++) { if (!Tables[i].Used) { return &Tables[i]; } }
            return nullptr;
        }

        int PageManager::GetPageTableIndex(PageTable* table)
        {
            for (uint32_t i = 0; i < PAGE_COUNT; i++) { if (&Tables[i] == table) { return i; } }
            return -1;
        }

        uint32_t PageManager::GetPhysical() { return Physical; }

        PageTable* PageManager::GetPageTables() { return Tables; }
    }
}