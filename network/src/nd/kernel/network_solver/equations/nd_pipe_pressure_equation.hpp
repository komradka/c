#include "nd_abstract_equation.hpp"

class nd_pipe_pressure_equation : public nd_equation
{
private:
    unsigned int link_in;
    unsigned int link_out;

    pipe_data *data;

public:
    nd_pipe_pressure_equation(unsigned int link_in,
                              unsigned int link_out,
                              pipe_data *data)
    {
        this->link_in = link_in;
        this->link_out = link_out;
        this->data = data;
    }

    error fill_rhs(nd_solution *solution, double &rhs) override;
    error fill_jacobian(nd_solution *solution, jacobian_row *row) override;

    std::string get_equation_name() override
    {
        return "Pipe pressure equation " + data->get_name();
    }
};