#include <string>
#include <map>
#include <optional>

#pragma once

enum class pipe_param_type
{
    pressure_begin,
    pressure_end,

    temperature_begin,
    temperature_end,

    volume_rate,

    pressure_drop,
    temperature_drop,

    length,
    pressure_gradient,
    pressure_error,

    COUNT
};

std::string get_name_for_gui(pipe_param_type e);

class pipe_param
{
private:
    std::map<pipe_param_type, double> segment_params;

public:
    pipe_param() = default;

    double &get_param(pipe_param_type e)
    {
        return segment_params[e];
    }
};