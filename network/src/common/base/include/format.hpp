#include <string>
#include <memory>
#include <cstdarg>
#include <sstream>


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

    static int string_to_int(std::string s)
    {
        return std::stoi(s);
    }

    static double string_to_double(std::string s)
    {
        return std::stod(s);
    }

    static void split_string(const std::string &str, std::vector<std::string> &words)
    {
        std::stringstream ss(str);
        std::string word;
        while (ss >> word)
        {
            words.push_back(word);
        }
    }
};

#pragma once

