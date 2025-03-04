#include "report_system.hpp"

void report_system::add_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check,
                                 report_widget *out_widget)
{
    simple_reporter *rep = new simple_reporter(std::move(check), out_widget);
    reporters.push_back(rep);
}

void report_system::add_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check,
                                 const char *file)
{
    simple_reporter *rep = new simple_reporter(std::move(check), file);
    reporters.push_back(rep);
}

std::string formating(const char *format, va_list va)
{
    va_list va_for_length;
    va_copy(va_for_length, va);
    int length = vsnprintf(nullptr, 0, format, va_for_length);
    va_end(va_for_length);

    std::string result;
    result.assign(length, '\0');
    vsprintf(&result[0], format, va);

    return result;
}

void report_system::print(message_type type, message_source source, message_category category, const char *format, ...)
{
    va_list va;
    va_start(va, format);

    std::string msg = formating(format, va);
    va_end(va);

    std::string res_msg = get_name(type) + get_name(source) + msg;

    for (simple_reporter *rep : reporters)
    {
        rep->print(type, source, category, res_msg);
    }
}   
