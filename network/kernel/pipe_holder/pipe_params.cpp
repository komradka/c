#include "pipe_params.hpp"

std::string get_name_for_gui(pipe_param_type e)
{
    switch (e)
    {
    case pipe_param_type::pressure_begin:
        return "Pressure at the segment begin";
    case pipe_param_type::pressure_end:
        return "Pressure at the segment end";
    case pipe_param_type::temperature_begin:
        return "Temperature at the segment begin";
    case pipe_param_type::temperature_end:
        return "Temperature at the segment end";
    case pipe_param_type::pressure_drop:
        return "Segment Pressure drop";
    case pipe_param_type::temperature_drop:
        return "Segment Temperature drop";
    case pipe_param_type::volume_rate:
        return "Volume Rate";
    case pipe_param_type::length:
        return "Segment Length";
    case pipe_param_type::pressure_gradient:
        return "Pressure Gradient";
    case pipe_param_type::pressure_error:
        return "Segment Error";
    case pipe_param_type::COUNT:
        return "";
    }
    return "";
}