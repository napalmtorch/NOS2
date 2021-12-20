#include <Kernel/Runtime/ELF.h>
#include <Kernel/Core/Kernel.h>
using namespace HAL;
using namespace System;
using namespace Graphics;

namespace Runtime
{
    namespace ELF
    {   
        const char ValidID[4] = { 0x7F, 'E', 'L', 'F' };

        bool Validate(ELFHeader* header) { return !memcmp(header->Identifier.ID, ValidID, 4); }

        bool Start(uint8_t* data, uint32_t size, Thread* thread)
        {
            if (!Validate((ELFHeader*)data)) { Debug::Error("File is not ELF format"); return false; }

            ELFHeader* header = (ELFHeader*)data;
            ELFProgramHeader* prog_header = (ELFProgramHeader*)(data + header->PHTable);

            if (header->Type != 2) { Debug::Error("ELF file is not executable, type = 0x%2x", header->Type); return false; }

            uint8_t* prog_data = (uint8_t*)Core::PhysicalMM.Allocate(size + 0x1000, true);

            for (uint32_t i = 0; i < header->PHEntryCount; i++, prog_header++)
            {
                switch (prog_header->Type)
                {
                    case 0: { break; }
                    case 1:
                    {
                        Debug::Info("ELF Load: OFFSET = 0x%8x, VIRT = 0x%8x, PHYS = 0x%8x, SIZE = %d", 
                                        prog_header->Offset, prog_header->Virtual, prog_header->Physical, prog_header->SegmentSize);
                        thread->ELFVirtual = prog_header->Virtual;
                        thread->ELFVirtual = (uint32_t)prog_data;
                        thread->PageMgr->Map(prog_header->Virtual, (uint32_t)prog_data);
                        memcpy((void*)prog_header->Virtual, (void*)((uint32_t)data + prog_header->Offset), prog_header->FileSize);
                        break;
                    }
                    default: { Debug::Error("Unsupported ELF program header type 0x%8x", prog_header->Type); return false; }
                }
            }

            return true;
        }
    }
}