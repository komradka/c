#include <optional>
#include <string>

#include "solver_settings_enum.hpp"

#pragma once

template <solver_settings Settings>
struct network_solver_setting_desc;

template <>
struct network_solver_setting_desc<solver_settings::use_temperature_equation>
{
    using data_type_t = bool;
    static constexpr auto default_type = false;
    static constexpr auto name = "use_temperature_equation";
    static constexpr auto name_for_gui = "Use Temperature";
};

template <>
struct network_solver_setting_desc<solver_settings::use_heat_balance_equation>
{
    using data_type_t = bool;
    static constexpr auto default_type = false;
    static constexpr auto name = "use_heat_balance_equation";
    static constexpr auto name_for_gui = "Use Heat Balance";
};

template <>
struct network_solver_setting_desc<solver_settings::solver_type>
{
    using data_type_t = solver_type;
    static constexpr auto default_type = solver_type::Newton;
    static constexpr auto name = "solver_type";
    static constexpr auto name_for_gui = "Solver Type";
};

template <>
struct network_solver_setting_desc<solver_settings::linear_solver_type>
{
    using data_type_t = linear_solver_type;
    static constexpr auto default_type = linear_solver_type::Iterative;
    static constexpr auto name = "linear_solver_type";
    static constexpr auto name_for_gui = "Linear Solver Type";
};

template <>
struct network_solver_setting_desc<solver_settings::preconditioner_type>
{
    using data_type_t = preconditioner_type;
    static constexpr auto default_type = preconditioner_type::none;
    static constexpr auto name = "preconditioner_type";
    static constexpr auto name_for_gui = "Preconditioner Type";
};

template <>
struct network_solver_setting_desc<solver_settings::rhs_eps>
{
    using data_type_t = double;
    static constexpr auto default_type = 1e-3;
    static constexpr auto name = "rhs_eps";
    static constexpr auto name_for_gui = "Newton Solver RHS norm";
};

template <>
struct network_solver_setting_desc<solver_settings::root_difference_eps>
{
    using data_type_t = double;
    static constexpr auto default_type = 1e-8;
    static constexpr auto name = "root_dif_eps";
    static constexpr auto name_for_gui = "Newton root difference norm";
};

template <>
struct network_solver_setting_desc<solver_settings::linear_solver_eps>
{
    using data_type_t = double;
    static constexpr auto default_type = 1e-5;
    static constexpr auto name = "linear_solver_eps";
    static constexpr auto name_for_gui = "Iterative linear solver residual";
};

template <>
struct network_solver_setting_desc<solver_settings::max_newton_iter>
{
    using data_type_t = int;
    static constexpr auto default_type = 50;
    static constexpr auto name = "max_newton_iter";
    static constexpr auto name_for_gui = "Maximum Newton Iteration";
};

template <>
struct network_solver_setting_desc<solver_settings::max_linear_iter>
{
    using data_type_t = int;
    static constexpr auto default_type = 150;
    static constexpr auto name = "max_linear_iter";
    static constexpr auto name_for_gui = "Maximum Linear Solver Iteration";
};

template <>
struct network_solver_setting_desc<solver_settings::initial_approximation_type>
{
    using data_type_t = initial_approximation_type;
    static constexpr auto default_type = initial_approximation_type::Advanced;
    static constexpr auto name = "initial_approximation_type";
    static constexpr auto name_for_gui = "Initial Approximation Type";
};

template <>
struct network_solver_setting_desc<solver_settings::rate_type>
{
    using data_type_t = rate_type;
    static constexpr auto default_type = rate_type::only_water;
    static constexpr auto name = "rate_type";
    static constexpr auto name_for_gui = "Fluid Model Type";
};

template <>
struct network_solver_setting_desc<solver_settings::visualize_initial_approximation>
{
    using data_type_t = bool;
    static constexpr auto default_type = false;
    static constexpr auto name = "visualize_initial_approximation";
    static constexpr auto name_for_gui = "Visualize Initial Approximation";
};
