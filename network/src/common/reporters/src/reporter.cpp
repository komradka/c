#include "reporter.hpp"

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

    stream->print(msg, (int)type % 3);
}
