#include <string>

#pragma once

enum class solver_settings
{
    use_temperature_equation,           // bool
    use_heat_balance_equation,          // bool
    visualize_initial_approximation,    // bool 

    solver_type,         // enum
    linear_solver_type,  // enum
    preconditioner_type, // enum

    rhs_eps,             // double
    root_difference_eps, // double
    linear_solver_eps,   // double

    max_newton_iter, // int
    max_linear_iter, // int

    initial_approximation_type, // enum

    rate_type, // enum

    COUNT
};

enum class solver_type
{
    Newton,
    Direct,

    COUNT 
};

enum class linear_solver_type
{
    Iterative,
    Direct,

    COUNT
};

enum class preconditioner_type
{
    none,

    COUNT
};

enum class initial_approximation_type
{
    Simple,
    Advanced,

    COUNT
};

enum class rate_type
{
    only_water,
    BO_model,

    COUNT
};

std::string get_name_for_gui(solver_type e);
std::string get_name_for_gui(linear_solver_type e);
std::string get_name_for_gui(preconditioner_type e);
std::string get_name_for_gui(initial_approximation_type e);
std::string get_name_for_gui(rate_type e);

std::string get_name(solver_type e);
std::string get_name(linear_solver_type e);
std::string get_name(preconditioner_type e);
std::string get_name(initial_approximation_type e);
std::string get_name(rate_type e);

solver_type get_solver_type_by_string(std::string e);
linear_solver_type get_liner_solver_type_by_string(std::string e);
preconditioner_type get_preconditioner_type_by_string(std::string e);
initial_approximation_type get_initial_approximation_type_by_string(std::string e);
rate_type get_rate_type_by_string(std::string e);
