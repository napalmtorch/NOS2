#pragma once
#include <Lib/Types.h>

namespace HAL
{
    namespace Memory
    {
        typedef struct
        {
            uint32_t  Physical;
            uint32_t  Virtual;
            uint32_t* Entries;
            bool      Used;
        } PACKED PageTable;

        class PageManager
        {
            private:
                uint32_t  Physical;
                PageTable Tables[1024];

            public:
                void          Init(uint32_t pd_start);
                void          Map(uint32_t v_addr, uint32_t p_addr, bool msg = true);
                void          Unmap(void* v_addr);
                void          WriteEntry(int index, uint32_t table);
                uint32_t      ReadEntry(int index);
                PageTable*    GetFreePageTable();
                int           GetPageTableIndex(PageTable* table);
                uint32_t      GetPhysical();
                PageTable*    GetPageTables();
        };

        namespace Paging
        {
            extern PageManager KernelDirectory;

            void Init();
            PageManager* Create();
        }
    }
}