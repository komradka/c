#include "nd_abstract_equation.hpp"

class nd_source_pressure_equation : public nd_equation
{
private:
    unsigned int link_num;
    source_data *data;

public:
    nd_source_pressure_equation(unsigned int link_num, source_data *data)
    {
        this->link_num = link_num;
        this->data = data;
    }

    error fill_rhs(nd_solution *solution,
                   double &rhs) override
    {
        double source_pressure = data->get_param<network_object_param::pressure>().value();

        rhs = solution->get_pressure(link_num) - source_pressure;

        return error(OK);
    }

    error fill_jacobian(nd_solution *,
                        jacobian_row &row) override
    {
        
    }
};