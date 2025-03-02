#include "reporter.hpp"

std::string get_name(message_type e)
{
    switch (e)
    {
    case message_type::MESSAGE:
        return "Message: ";
    case message_type::WARNING:
        return "Warning: ";
    case message_type::ERR:
        return "Error: ";
    case message_type::DEBUG:
        return "D_Message: ";
    case message_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(message_source e)
{
    switch (e)
    {
    case message_source::MAIN:
        return "Project: ";
    case message_source::ND:
        return "Network Designer: ";
    case message_source::PIPE:
        return "Pipe Designer: ";
    case message_source::PVT:
        return "PVT Designer: ";
    case message_source::SIM:
        return "Simulator: ";
    case message_source::WF:
        return "Workflow: ";
    case message_source::COUNT:
        return "";
    }
    return "";
}

simple_reporter::simple_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check,
                                 const char *file)
{
    this->check = std::move(check);
    stream = new project_stream();
    error ret = stream->open(file);

    if (!ret.is_ok())
    {
        printf("%s\n", ret.description_cstr());
        abort();
    }
}

simple_reporter::simple_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check,
                                 report_widget *out_widget)
{
    this->check = std::move(check);
    stream = new project_stream();
    stream->open(out_widget);
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

void simple_reporter::print(message_type type, message_source source, message_category category, const char *format, ...)
{
    if (!check(type, source, category))
    {
        return;
    }

    if (!is_debug && type == message_type::DEBUG)
    {
        return;
    }

    va_list va;
    va_start(va, format);

    std::string msg = formating(format, va);
    va_end(va);

    std::string res_msg = get_name(type) + get_name(source) + msg;

    stream->print(res_msg, (int)type);
}