#include "nd_abstract_equation.hpp"

class nd_equality_rate_equation : public nd_equation
{
private:
    unsigned int link_in;
    unsigned int link_out;
    object_data *data;

public:
    nd_equality_rate_equation(unsigned int link_in, unsigned int link_out, object_data *data)
    {
        this->link_in = link_in;
        this->link_out = link_out;
        this->data = data;
    }

    error fill_rhs(nd_solution *solution,
                   double &rhs) override
    {
        double q_in = solution->get_wrat(link_in);
        double q_out = solution->get_wrat(link_out);

        rhs = q_out - q_in;

        return error(OK);
    }

    error fill_jacobian(nd_solution *,
                        jacobian_row *row) override
    {
        double D_F_D_q_out = 1;
        double D_F_D_q_in = -1;

        row->fill_row(link_in, link_out, nullptr, nullptr, &D_F_D_q_in, &D_F_D_q_out, nullptr, nullptr);

        return error(OK);
    }

    std::string get_equation_name() override
    {
        return "Equality rate equation " + data->get_name();
    }
};