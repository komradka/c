#include "object_param_enum.hpp"

std::string get_name_for_gui(pipe_type e)
{
    switch (e)
    {
    case pipe_type::constant_coef:
        return "Constant pressure drop coefficient";
    case pipe_type::correlation:
        return "By correlation [WIP]";
    case pipe_type::COUNT:
        return "UNKNOWN";
    }
    return " ";
}