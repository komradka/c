#include "project_stream.hpp"
#include <vector>
#include <memory>
#include <cstdarg>
#include <string>
#include <vector>
#include <functional>

#pragma once

enum class message_type
{
    MESSAGE,
    WARNING,
    ERR,
    DEBUG,

    COUNT
};

enum class message_source
{
    ND,
    SIM,
    PVT,
    WF,
    PIPE,
    MAIN,

    COUNT
};

enum class message_category
{
    LOG,
    FILE,
    STD,
    ALL,

    COUNT
};

std::string get_name(message_type e);
std::string get_name(message_source e);

class simple_reporter
{
private:
    project_stream *stream;
    bool is_debug = true;

    std::function<bool(message_type type, message_source source, message_category category)> check;

public:
    simple_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check,
                    const char *file = nullptr); // stdout by default

    simple_reporter(std::function<bool(message_type type, message_source source, message_category category)> &&check,
                    report_widget *out_widget);

    ~simple_reporter()
    {
        delete stream;
    }

    void print(message_type type, message_source source, message_category category, const char *format, ...);
};