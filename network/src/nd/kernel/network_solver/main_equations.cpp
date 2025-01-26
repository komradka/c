#include "main_equations.hpp"

error main_equations::set_pressure_equation(std::vector<unsigned int> links, nd_equation *eq)
{
    std::vector<unsigned int> eq_pos;
    eq_pos.resize(links.size(), 0);

    std::transform(links.begin(), links.end(), eq_pos.begin(), [&](unsigned int link)
                   { return solution->get_pressure_eq_pos(link); });

    RETURN_IF_FAIL(set_main_equation(eq_pos, eq));

    return error(OK);
}

error main_equations::set_rate_equation(std::vector<unsigned int> links, nd_equation *eq)
{
    std::vector<unsigned int> eq_pos;
    eq_pos.resize(links.size(), 0);

    std::transform(links.begin(), links.end(), eq_pos.begin(), [&](unsigned int link)
                   { return solution->get_rate_eq_pos(link); });

    RETURN_IF_FAIL(set_main_equation(eq_pos, eq));

    return error(OK);
}

error main_equations::set_temp_equation(std::vector<unsigned int> links, nd_equation *eq)
{
    std::vector<unsigned int> eq_pos;
    eq_pos.resize(links.size(), 0);

    std::transform(links.begin(), links.end(), eq_pos.begin(), [&](unsigned int link)
                   { return solution->get_temp_eq_pos(link); });

    RETURN_IF_FAIL(set_main_equation(eq_pos, eq));

    return error(OK);
}