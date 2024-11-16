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
        return "Simple (not recommended) [WIP]";
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