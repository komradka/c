#include "nd_abstract_equation.hpp"

#pragma once

class nd_joint_rate_equation : public nd_equation
{
private:
    std::vector<unsigned int> links;
    unsigned int inlet_links_count;
    unsigned int outlet_links_count;

public:
    nd_joint_rate_equation(unsigned int inlet_links_count, unsigned int outlet_links_count, std::vector<unsigned int> &links)
    {
        this->links = links;
        this->inlet_links_count = inlet_links_count;
        this->outlet_links_count = outlet_links_count;
    }

    error fill_rhs(nd_solution *solution, double &rhs)
    {
        rhs = 0;
        for (unsigned int i = 0; i < inlet_links_count; i++)
        {
            rhs += solution->get_wrat(links[i]);
        }

        for (unsigned int i = inlet_links_count; i < inlet_links_count + outlet_links_count; i++)
        {
            rhs -= solution->get_wrat(links[i]);
        }

        return error(OK);
    }

    error fill_jacobian(nd_solution *,
                        jacobian_row *row) override
    {
        row->fill_row_for_joint(links, inlet_links_count, outlet_links_count);

        return error(OK);
    }

    std::string get_equation_name() override
    {
        return "Joint rate equation";
    }
};