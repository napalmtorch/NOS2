#pragma once
#include <Lib/Types.h>

EXTC
{
    void* memchr(const void* str, int c, uint32_t n);
    int   memcmp(const void* str1, const void* str2, uint32_t n);
    int   memicmp(const void* str1, const void* str2, uint32_t n);
    void* memcpy(void* dest, const void* src, uint32_t n);
    void* memmove(void* dest, const void* src, uint32_t n);
    void* memset(void* dest, int c, uint32_t n);
}