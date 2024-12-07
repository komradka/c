#include <string>

#pragma once

enum class pipe_type
{
    constant_coef,
    correlation,

    COUNT
};

enum class pipe_correlation
{
    old,
    Moody,

    COUNT
};

pipe_type get_pipe_type_by_storage_name(std::string e);
std::string get_storage_name(pipe_type e);
std::string get_name_for_gui(pipe_type e);
std::string get_name_for_gui(pipe_correlation e);