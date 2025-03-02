#include "project_stream.hpp"
#include <vector>
#include <memory>
#include <cstdarg>

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

class simple_reporter
{
private:
    project_stream *stream;
    bool is_debug;

public:
    simple_reporter(const char *file = nullptr); // stdout by default
    simple_reporter(report_widget *out_widget);

    void print(message_type type, message_source source, message_category category, const char *format, ...);
};