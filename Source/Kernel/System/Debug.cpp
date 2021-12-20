#include <Kernel/System/Debug.h>
#include <Kernel/Core/Kernel.h>
using namespace Graphics;
using namespace Runtime;

namespace System
{
    namespace Debug
    {
        DebugMode CurrentMode;

        void SetMode(DebugMode mode) { CurrentMode = mode; }
        DebugMode GetMode() { return CurrentMode; }

        bool IsSerial(DebugMode mode) { return (mode == DebugMode::Serial   || mode == DebugMode::All); }
        bool IsTerm(DebugMode mode)   { return (mode == DebugMode::Terminal || mode == DebugMode::All); }

        void WriteChar(char c) { WriteChar(c, CurrentMode); }

        void WriteChar(char c, Graphics::Color4 fg) { WriteChar(c, fg, CurrentMode); }

        void WriteChar(char c, DebugMode mode)
        {
            if (IsSerial(mode)) { HAL::Serial::WriteChar(c); }
            if (IsTerm(mode)) { }
        }

        void WriteChar(char c, Graphics::Color4 fg, DebugMode mode)
        {
            if (IsSerial(mode)) { HAL::Serial::WriteChar(c, fg); }
            if (IsTerm(mode)) { }
        }
        
        void Write(const char* str) { Write(str, CurrentMode); }

        void Write(const char* str, Graphics::Color4 fg) { Write(str, fg, CurrentMode); }

        void Write(const char* str, DebugMode mode)
        {
            if (IsSerial(mode)) { HAL::Serial::Write(str); }
            if (IsTerm(mode)) { }
        }

        void Write(const char* str, Graphics::Color4 fg, DebugMode mode)
        {
            if (IsSerial(mode)) { HAL::Serial::Write(str, fg); }
            if (IsTerm(mode)) { }
        }

        void WriteLine(const char* str) { WriteLine(str, CurrentMode); }

        void WriteLine(const char* str, Graphics::Color4 fg) { WriteLine(str, fg, CurrentMode); }

        void WriteLine(const char* str, DebugMode mode)
        {
            Write(str, mode);
            WriteChar('\n', mode);
        }

        void WriteLine(const char* str, Graphics::Color4 fg, DebugMode mode)
        {
            Write(str, fg, mode);
            WriteChar('\n', mode);
        }

        void PrintFormatted(const char* str, va_list args) { PrintFormatted(str, CurrentMode, args); }
        
        void PrintFormatted(const char* str, ...)
        {
            va_list args;
            va_start(args, str);
            PrintFormatted(str, CurrentMode, args);
            va_end(args);
        }

        void PrintFormatted(const char* str, DebugMode mode, va_list args)
        {
            while (*str != 0)
            {
                if (*str == '%')
                {
                    str++;
                    if (*str == '%') { WriteChar('%', mode); }
                    if (*str == 'c') { WriteChar((char)va_arg(args, int), mode); }
                    else if (*str == 'd' || *str == 'u')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        Write(itoa(dec, str, 10), mode);
                    }
                    else if (*str == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        Write(ltoa(dec, str, 10), mode);
                    }
                    else if (*str == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        Write(ltoa(dec, str, 16), mode);
                    }
                    else if (*str == '2')
                    {
                        str++;
                        if (*str != 'x' && *str != 'X') { WriteChar(*str, mode); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Write(strhex(num, str, false, 1), mode);
                        }
                    }
                    else if (*str == '4')
                    {
                        str++;
                        if (*str != 'x' && *str != 'X') { WriteChar(*str, mode); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Write(strhex(num, str, false, 2), mode);
                        }
                    }
                    else if (*str == '8')
                    {
                        str++;
                        if (*str != 'x' && *str != 'X') { WriteChar(*str, mode); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Write(strhex(num, str, false, 4), mode);
                        }
                    }
                    else if (*str == 'f')
                    {
                        double num = va_arg(args, double);
                        char str[32];
                        memset(str, 0, 32);
                        Write(ftoa(num, str, 2), mode);
                    }
                    else if (*str == 's')
                    {
                        char* val = va_arg(args, char*);
                        Write(val, mode);
                    }
                    else { WriteChar(*str, mode); }
                }
                else { WriteChar(*str, mode); }
                str++;
            }
        }

        void PrintFormatted(const char* str, DebugMode mode, ...)
        {
            va_list args;
            va_start(args, mode);
            PrintFormatted(str, mode, args);
            va_end(args);
        }

        void Header(const char* str, Color4 color, DebugMode mode)
        {
            Write("[", mode);
            Write(str, color, mode);
            Write("] ", mode);
        }

        void Info(const char* str, ...)
        {
            va_list args;
            va_start(args, str);
            Header("  >>  ", Color4::Cyan, CurrentMode);
            PrintFormatted(str, CurrentMode, args);
            WriteChar('\n', CurrentMode);
            va_end(args);
        }

        void Info(const char* str, DebugMode mode, ...)
        {
            va_list args;
            va_start(args, mode);
            Header("  >>  ", Color4::Cyan, mode);
            PrintFormatted(str, mode, args);
            WriteChar('\n', mode);
            va_end(args);
        }

        void OK(const char* str, ...)
        {
            va_list args;
            va_start(args, str);
            Header("  OK  ", Color4::Green, CurrentMode);
            PrintFormatted(str, CurrentMode, args);
            WriteChar('\n', CurrentMode);
            va_end(args);
        }

        void OK(const char* str, DebugMode mode, ...)
        {
            va_list args;
            va_start(args, mode);
            Header("  OK  ", Color4::Green, mode);
            PrintFormatted(str, mode, args);
            WriteChar('\n', mode);
            va_end(args);
        }

        void Warning(const char* str, ...)
        {
            va_list args;
            va_start(args, str);
            Header("  ??  ", Color4::Yellow, CurrentMode);
            PrintFormatted(str, CurrentMode, args);
            WriteChar('\n', CurrentMode);
            va_end(args);
        }

        void Warning(const char* str, DebugMode mode, ...)
        {
            va_list args;
            va_start(args, mode);
            Header("  ??  ", Color4::Yellow, mode);
            PrintFormatted(str, mode, args);
            WriteChar('\n', mode);
            va_end(args);
        }

        void Error(const char* str, ...)
        {
            va_list args;
            va_start(args, str);
            Header("  !!  ", Color4::Red, CurrentMode);
            PrintFormatted(str, CurrentMode, args);
            WriteChar('\n', CurrentMode);
            va_end(args);
        }

        void Error(const char* str, DebugMode mode, ...)
        {
            va_list args;
            va_start(args, mode);
            Header("  !!  ", Color4::Red, mode);
            PrintFormatted(str, mode, args);
            WriteChar('\n', mode);
            va_end(args);
        }
    }
}