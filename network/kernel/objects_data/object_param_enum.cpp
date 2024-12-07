#include "object_param_enum.hpp"

std::string get_storage_name(pipe_type e)
{
    switch (e)
    {
    case pipe_type::constant_coef:
        return "CONST";
    case pipe_type::correlation:
        return "CORR";
    case pipe_type::COUNT:
        return "UNK";
    }
    return " ";
}

pipe_type get_pipe_type_by_storage_name(std::string e)
{
    if (e == "CONST")
        return pipe_type::constant_coef;
    if (e == "CORR")
        return pipe_type::correlation;
    return pipe_type::COUNT;
}

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

std::string get_name_for_gui(pipe_correlation e)
{
    switch (e)
    {
    case pipe_correlation::old:
        return "Old correlation [Not Recommended]";
    case pipe_correlation::Moody:
        return "Moody correlation";
    case pipe_correlation::COUNT:
        return "";
    }

    return "";
}