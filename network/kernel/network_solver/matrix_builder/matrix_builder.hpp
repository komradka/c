#include "jacobian_row.hpp"
#include "../main_equations.hpp"
#include <memory>

#pragma once

class matrix_builder
{
private:
    main_equations *equations;
    nd_solution *solution;

public:
    matrix_builder(main_equations *equations, nd_solution *solution)
    {
        this->equations = equations;
        this->solution = solution;
    }

    jacobian_row *make_row(unsigned int pos)
    {
        unsigned int var_count = solution->get_main_vars_count();

        jacobian_row *row = new jacobian_row(pos, var_count);

        equations->get_equation(pos)->fill_jacobian(solution, row);

        return row;
    }
};