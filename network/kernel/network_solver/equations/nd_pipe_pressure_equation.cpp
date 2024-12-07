#include "nd_pipe_pressure_equation.hpp"

error nd_pipe_pressure_equation::fill_rhs(nd_solution *solution, double &rhs)
{
    pipe_type type = data->get_param<network_object_param::pipe_type>();

    double p_in = solution->get_pressure(link_in);
    double p_out = solution->get_pressure(link_out);

    if (type == pipe_type::constant_coef)
    {
        double coef = data->get_param<network_object_param::pipe_pressure_drop_coef>().value();

        double length = data->get_param<network_object_param::pipe_len>().value();

        double pressure_drop = length * coef;

        rhs = p_in - p_out - pressure_drop;

        return error(OK);
    }

    rhs = p_in - p_out;
    return error(OK);
}

error nd_pipe_pressure_equation::fill_jacobian(nd_solution *, jacobian_row *row)
{
    double D_F_D_p_out = -1.;
    double D_F_D_p_in = 1.;

    row->fill_row(link_in, link_out, &D_F_D_p_out, &D_F_D_p_in, 0, 0, 0, 0);

    return error(OK);
}