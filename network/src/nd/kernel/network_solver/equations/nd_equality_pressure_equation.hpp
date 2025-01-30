#include "nd_abstract_equation.hpp"

class nd_equality_pressure_equation : public nd_equation
{
private:
    unsigned int link_in;
    unsigned int link_out;
    object_data *data;

public:
    nd_equality_pressure_equation(unsigned int link_in, unsigned int link_out, object_data *data)
    {
        this->link_in = link_in;
        this->link_out = link_out;
        this->data = data;
    }

    error fill_rhs(nd_solution *solution,
                   double &rhs) override
    {
        double p_in = solution->get_pressure(link_in);
        double p_out = solution->get_pressure(link_out);

        rhs = p_out - p_in;

        return error(OK);
    }

    error fill_jacobian(nd_solution *,
                        jacobian_row *row) override
    {
        double D_F_D_p_out = 1;
        double D_F_D_p_in = -1;

        row->fill_row(link_in, link_out, &D_F_D_p_in, &D_F_D_p_out, nullptr, nullptr, nullptr, nullptr);

        return error(OK);
    }

    std::string get_equation_name() override
    {
        return "Equality pressure equation " + data->get_name();
    }
};