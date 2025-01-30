#include "nd_abstract_equation.hpp"

class nd_source_rate_equation : public nd_equation
{
private:
    unsigned int link_num;
    object_data *data;

public:
    nd_source_rate_equation(unsigned int link_num, object_data *data)
    {
        this->link_num = link_num;
        this->data = data;
    }

    error fill_rhs(nd_solution *solution,
                   double &rhs) override
    {
        double rate = data->get_param<network_object_param::wrat>().value();

        rhs = solution->get_wrat(link_num) - rate;

        return error(OK);
    }

    error fill_jacobian(nd_solution *,
                        jacobian_row *row) override
    {
        double D_F_D_q_out = 1;

        row->fill_row(link_num, &D_F_D_q_out, nullptr, nullptr);

        return error(OK);
    }

    std::string get_equation_name() override
    {
        return "Constant rate equation " + data->get_name();
    }
};