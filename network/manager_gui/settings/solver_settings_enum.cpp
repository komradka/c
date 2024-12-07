#include "solver_settings_enum.hpp"

std::string get_name_for_gui(solver_type e)
{
    switch (e)
    {
    case solver_type::Direct:
        return "Direct solver (not recommended) [WIP]";
    case solver_type::Newton:
        return "Iterative [WIP]";
    case solver_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(linear_solver_type e)
{
    switch (e)
    {
    case linear_solver_type::Direct:
        return "Direct (not recommended) [WIP]";
    case linear_solver_type::Iterative:
        return "Iterative [WIP]";
    case linear_solver_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(preconditioner_type e)
{
    switch (e)
    {
    case preconditioner_type::none:
        return "None";
    case preconditioner_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(initial_approximation_type e)
{
    switch (e)
    {
    case initial_approximation_type::Advanced:
        return "Advanced [WIP]";
    case initial_approximation_type::Simple:
        return "Simple (not recommended)";
    case initial_approximation_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(rate_type e)
{
    switch (e)
    {
    case rate_type::only_water:
        return "Only Water [WIP]";
    case rate_type::BO_model:
        return "Black Oil Model [WIP]";
    case rate_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(solver_type e)
{
    switch (e)
    {
    case solver_type::Direct:
        return "DIR";
    case solver_type::Newton:
        return "IT";
    case solver_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(linear_solver_type e)
{
    switch (e)
    {
    case linear_solver_type::Direct:
        return "DIR";
    case linear_solver_type::Iterative:
        return "IT";
    case linear_solver_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(preconditioner_type e)
{
    switch (e)
    {
    case preconditioner_type::none:
        return "NON";
    case preconditioner_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(initial_approximation_type e)
{
    switch (e)
    {
    case initial_approximation_type::Advanced:
        return "ADV";
    case initial_approximation_type::Simple:
        return "SIMPL";
    case initial_approximation_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(rate_type e)
{
    switch (e)
    {
    case rate_type::only_water:
        return "WAT";
    case rate_type::BO_model:
        return "BO";
    case rate_type::COUNT:
        return "";
    }
    return "";
}

solver_type get_solver_type_by_string(std::string e)
{
    if (e == "DIR")
        return solver_type::Direct;
    if (e == "IT")
        return solver_type::Newton;

    return solver_type::COUNT;
}
linear_solver_type get_liner_solver_type_by_string(std::string e)
{
    if (e == "DIR")
        return linear_solver_type::Direct;
    if (e == "IT")
        return linear_solver_type::Iterative;

    return linear_solver_type::COUNT;
}
preconditioner_type get_preconditioner_type_by_string(std::string e)
{
    if (e == "NON")
        return preconditioner_type::none;

    return preconditioner_type::COUNT;
}
initial_approximation_type get_initial_approximation_type_by_string(std::string e)
{
    if (e == "ADV")
        return initial_approximation_type::Advanced;
    if (e == "SIMPL")
        return initial_approximation_type::Simple;
    return initial_approximation_type::COUNT;
}
rate_type get_rate_type_by_string(std::string e)
{
    if (e == "WAT")
        return rate_type::only_water;
    if (e == "BO")
        return rate_type::BO_model;
    return rate_type::COUNT;
}