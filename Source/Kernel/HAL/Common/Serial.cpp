#include <Kernel/HAL/Common/Serial.h>
#include <Kernel/Core/Kernel.h>
using namespace Graphics;

namespace HAL
{
    namespace Serial
    {
        SerialPort CurrentPort = SerialPort::Disabled;

        void SetPort(SerialPort port) { CurrentPort = port; }

        SerialPort GetPort() { return CurrentPort; }
        
        const char* GetPortString(SerialPort port)
        {
            switch (port)
            {
                case SerialPort::Disabled: { return "DISABLED"; }
                case SerialPort::COM1: { return "COM1"; }
                case SerialPort::COM2: { return "COM2"; }
                case SerialPort::COM3: { return "COM3"; }
                case SerialPort::COM4: { return "COM4"; }
                default: { return "ERROR"; }
            }
        }

        bool HasRecieved()
        {
            if (CurrentPort == SerialPort::Disabled) { return false; }
            return Ports::Read8((uint16_t)CurrentPort + 5) & 1;
        }

        bool CanSend()
        {
            if (CurrentPort == SerialPort::Disabled) { return false; }
            return Ports::Read8((uint16_t)CurrentPort + 5) & 0x20;
        }

        char Read()
        {
            if (CurrentPort == SerialPort::Disabled) { return false; }
            while (!HasRecieved());
            return Ports::Read8((uint16_t)CurrentPort);
        }

        void WriteChar(char c)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            while (!CanSend());
            Ports::Write8((uint16_t)CurrentPort, c);
        }
        
        void WriteChar(char c, Color4 color)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            SetColor(color);
            WriteChar(c);
            SetColor(Color4::White);
        }

        void Write(const char* str)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            int i = 0;
            while (str[i] != 0) { WriteChar(str[i]); i++; }
        }
        
        void Write(const char* str, Color4 color)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            SetColor(color);
            Write(str);
            SetColor(Color4::White);
        }

        void WriteLine(const char* str)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            Write(str);
            WriteChar('\n');
        }

        void WriteLine(const char* str, Color4 color)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            Write(str, color);
            WriteChar('\n');
        }

        void SetColor(Graphics::Color4 color)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }

            uint8_t c = (uint8_t)color;
            switch (c)
            {
                case 0x0: { Write("\033[34");  break; }
                case 0x1: { Write("\033[34m"); break; }
                case 0x2: { Write("\033[32m"); break; }
                case 0x3: { Write("\033[36m"); break; }
                case 0x4: { Write("\033[31m"); break; }
                case 0x5: { Write("\033[35m"); break; }
                case 0x6: { Write("\033[33m"); break; }
                case 0x7: { Write("\033[37m"); break; }
                case 0x8: { Write("\033[37m"); break; }
                case 0x9: { Write("\033[34m"); break; }
                case 0xA: { Write("\033[32m"); break; }
                case 0xB: { Write("\033[36m"); break; }
                case 0xC: { Write("\033[31m"); break; }
                case 0xD: { Write("\033[35m"); break; }
                case 0xE: { Write("\033[33m"); break; }
                case 0xF: { Write("\033[37m"); break; }
                default:  { Write("\033[0m");  break; }
            }
        }

        void PrintFormatted(const char* str, va_list args)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            while (*str != 0)
            {
                if (*str == '%')
                {
                    str++;
                    if (*str == '%') { WriteChar('%'); }
                    if (*str == 'c') { WriteChar((char)va_arg(args, int)); }
                    else if (*str == 'd' || *str == 'u')
                    {
                        int dec = va_arg(args, int);
                        char str[16];
                        memset(str, 0, 16);
                        Write(itoa(dec, str, 10));
                    }
                    else if (*str == 'u')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        Write(ltoa(dec, str, 10));
                    }
                    else if (*str == 'x')
                    {
                        uint32_t dec = va_arg(args, uint32_t);
                        char str[16];
                        memset(str, 0, 16);
                        Write(ltoa(dec, str, 16));
                    }
                    else if (*str == '2')
                    {
                        str++;
                        if (*str != 'x' && *str != 'X') { WriteChar(*str); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Write(strhex(num, str, false, 1));
                        }
                    }
                    else if (*str == '4')
                    {
                        str++;
                        if (*str != 'x' && *str != 'X') { WriteChar(*str); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Write(strhex(num, str, false, 2));
                        }
                    }
                    else if (*str == '8')
                    {
                        str++;
                        if (*str != 'x' && *str != 'X') { WriteChar(*str); }
                        else
                        {
                            uint32_t num = va_arg(args, uint32_t);
                            char str[16];
                            memset(str, 0, 16);
                            Write(strhex(num, str, false, 4));
                        }
                    }
                    else if (*str == 'f')
                    {
                        /* TODO : implement float to string */
                    }
                    else if (*str == 's')
                    {
                        char* val = va_arg(args, char*);
                        Write(val);
                    }
                    else { WriteChar(*str); }
                }
                else { WriteChar(*str); }
                str++;
            }
        }

        void PrintFormatted(const char* str, ...)
        {
            if (CurrentPort == SerialPort::Disabled) { return; }
            va_list args;
            va_start(args, str);
            PrintFormatted(str, args);
            va_end(args);
        }
    }
}