#include "error.hpp"
#include "nd/kernel/network_solver/nd_solution.hpp"
#include "nd/kernel/network_solver/matrix_builder/jacobian_row.hpp"

#include <string>

#pragma once

class nd_equation
{
public:
    nd_equation() = default;

    virtual error fill_rhs(nd_solution *solution,
                           double &rhs) = 0;

    virtual error fill_jacobian(nd_solution *solution,
                                jacobian_row *row) = 0;

    virtual std::string get_equation_name() = 0;
};
