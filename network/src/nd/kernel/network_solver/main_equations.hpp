#include <vector>
#include "equations/nd_abstract_equation.hpp"

#pragma once
class main_equations
{
private:
    std::vector<nd_equation *> equations;
    unsigned int main_equations_count;
    nd_solution *solution;

public:
    main_equations(nd_solution *solution)
    {
        this->solution = solution;
        this->main_equations_count = solution->get_main_vars_count();

        equations.resize(main_equations_count, nullptr);
    }

    error set_main_equation(std::vector<unsigned int> pos, nd_equation *eq)
    {
        for (auto i : pos)
        {
            if (i > main_equations_count)
            {
                return error("Wrong place for equation " + eq->get_equation_name());
            }
            if (equations[i] == nullptr)
            {
                equations[i] = eq;
                return error(OK);
            }
        }

        return error("No place for equation " + eq->get_equation_name());
    }

    void print_eq()
    {
        for (unsigned int i = 0; i < main_equations_count; i++)
        {
            cout << equations[i]->get_equation_name() << endl;
        }
    }

    error set_pressure_equation(std::vector<unsigned int> links, nd_equation *eq);

    error set_rate_equation(std::vector<unsigned int> links, nd_equation *eq);

    error set_temp_equation(std::vector<unsigned int> links, nd_equation *eq);

    nd_equation *get_equation(unsigned int pos)
    {
        return equations[pos];
    }
};