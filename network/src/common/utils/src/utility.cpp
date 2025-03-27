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

double get_special_value()
{
    return std::nan("1");
}

bool is_special_value(double x)
{
    return std::isnan(x);
}

std::string make_error(const std::string &err_message, const int line)
{
    return "Wrong file format: " + err_message + ".\n" + "In line - " + std::to_string(line);
}
