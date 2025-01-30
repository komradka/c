#include "utility.hpp"

std::string get_name_for_gui(int e)
{
    return std::to_string(e);
}

std::string get_name_for_gui(double e)
{
    return std::to_string(e);
}

std::string get_name_for_gui(bool e)
{
    return e ? "True" : "False";
}

std::string get_name(int e)
{
    return std::to_string(e);
}

std::string get_name(double e)
{
    return std::to_string(e);
}

std::string get_name(bool e)
{
    return e ? "True" : "False";
}