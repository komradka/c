#include <string>

#pragma once

enum class pipe_type
{
    constant_coef,
    correlation,

    COUNT
};

std::string get_name_for_gui(pipe_type e);