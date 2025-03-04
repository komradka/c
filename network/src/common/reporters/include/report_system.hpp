#include "reporter.hpp"

#pragma once

class report_system
{
private:
    std::vector<simple_reporter *> reporters;

public:
    report_system() = default;

    ~report_system()
    {
        for (unsigned i = 0; i < reporters.size(); i++)
        {
            delete reporters[i];
        }
    }

    void add_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check, report_widget *out_widget);
    void add_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check, const char *file);

    void print(message_type type, message_source source, message_category category, const char *format, ...);
};
