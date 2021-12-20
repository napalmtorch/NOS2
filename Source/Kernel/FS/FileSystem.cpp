#include <Kernel/FS/FileSystem.h>

namespace FileSystem
{
    void FSBlockTable::Init(FSHost* host)
    {
        Host = host;
    }

    void FSBlockTable::Print()
    {

    }

    FSBlockEntry FSBlockTable::Read(int index)
    {

    }

    bool FSBlockTable::Write(int index, FSBlockEntry entry)
    {

    }

    bool FSBlockTable::Copy(FSBlockEntry dest, FSBlockEntry src)
    {

    }

    bool FSBlockTable::Fill(FSBlockEntry entry, uint8_t value)
    {

    }

    FSBlockEntry FSBlockTable::Allocate(uint32_t sectors)
    {

    }

    bool FSBlockTable::Free(FSBlockEntry entry)
    {

    }

    void FSBlockTable::MergeFreeBlocks()
    {

    }

    FSBlockEntry FSBlockTable::GetNearest(FSBlockEntry entry)
    {

    }

    FSBlockEntry FSBlockTable::CreateEntry(uint32_t start, uint32_t count, uint8_t state)
    {

    }

    bool FSBlockTable::DeleteEntry(FSBlockEntry entry)
    {

    }
    
    FSBlockEntry FSBlockTable::Get(int index)
    {

    }

    bool FSBlockTable::ValidateSector(uint32_t sector)
    {

    }

    int FSBlockTable::GetIndex(FSBlockEntry entry)
    {

    }

    int FSBlockTable::GetFreeIndex()
    {

    }

    uint32_t FSBlockTable::GetSectorFromIndex(int index)
    {

    }

    uint32_t FSBlockTable::GetOffsetFromIndex(uint32_t sector, int index)
    {

    }

    // ----------------------------------------------------------------------------

    void FSFileTable::Init(FSHost* host)
    {
        Host = host;
    }

    void FSFileTable::Print()
    {

    }

    FSDirectory FSFileTable::ReadDirectory(int index)
    {

    }

    FSFile FSFileTable::ReadFile(int index)
    {

    }

    bool FSFileTable::WriteDirectory(int index, FSDirectory dir)
    {

    }

    bool FSFileTable::WriteFile(int index, FSFile file)
    {
        
    }

    FSDirectory FSFileTable::CreateDirectory(FSDirectory dir)
    {

    }

    FSFile FSFileTable::CreateFile(FSFile file)
    {

    }

    bool FSFileTable::DeleteDirectory(FSDirectory dir)
    {

    }

    bool FSFileTable::DeleteFile(FSFile file)
    {

    }

    bool FSFileTable::ValidateSector(uint32_t sector)
    {

    }

    int FSFileTable::GetDirectoryIndex(FSDirectory dir)
    {

    }

    int FSFileTable::GetFileIndex(FSFile file)
    {

    }

    int FSFileTable::GetFreeIndex()
    {

    }

    uint32_t FSFileTable::GetSectorFromIndex(int index)
    {

    }

    uint32_t FSFileTable::GetOffsetFromIndex(uint32_t sector, int index)
    {

    }

    // -----------------------------------------------------------------------------------------

    void FSHost::Mount()
    {

    }

    void FSHost::Unmount()
    {

    }

    void FSHost::Format(uint32_t size, bool wipe)
    {

    }

    void FSHost::Wipe(uint32_t size)
    {

    }

    void FSHost::CreateInfo(uint32_t size)
    {

    }

    void FSHost::ReadInfo()
    {

    }

    void FSHost::WriteInfo()
    {

    }

    FSInfo FSHost::GetInfo() { }

    bool FSHost::DirectoryEquals(FSDirectory a, FSDirectory b)
    {

    }

    bool FSHost::FileEquals(FSFile a, FSFile b)
    {

    }

    FSDirectory FSHost::GetParentFromPath(const char* path)
    {

    }

    FSDirectory FSHost::GetDirectoryByName(const char* path)
    {

    }

    FSFile FSHost::GetFileByName(const char* path)
    {

    }

    int FSHost::GetDirectoryIndex(FSDirectory dir)
    {

    }

    int FSHost::GetFileIndex(FSFile file)
    {

    }

    char* FSHost::GetNameFromPath(const char* path)
    {

    }

    char* FSHost::GetParentPathFromPath(const char* path)
    {

    }

    FSFile FSHost::CreateFile(const char* path, uint32_t size)
    {

    }

    FSFile FSHost::ReadFile(const char* path)
    {

    }

    bool FSHost::WriteFile(const char* path, uint8_t* data, uint32_t len)
    {

    }

    uint32_t FSHost::GetSectorsFromBytes(uint32_t bytes)
    {
        
    }
}