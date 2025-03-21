#include <string>
#include <memory>
#include <cstdarg>


class fmt
{
public:
    static std::string formating(const char *format, ...)
    {
        va_list va;
        va_start(va, format);

        va_list va_for_length;
        va_copy(va_for_length, va);
        int length = vsnprintf(nullptr, 0, format, va_for_length);
        va_end(va_for_length);

        std::string result;
        result.assign(length, '\0');
        vsprintf(&result[0], format, va);
        va_end(va);

        return result;
    }
};

#pragma once

