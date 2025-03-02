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

void simple_reporter::print(message_type type, message_source source, message_category category, std::string msg)
{
    if (!check(type, source, category))
    {
        return;
    }

    if (!is_debug && type == message_type::DEBUG)
    {
        return;
    }

    stream->print(msg, (int)type);
}