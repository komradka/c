#include <string>

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
