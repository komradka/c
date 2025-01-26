#include "phys_q_type.hpp"
#include "network_solver/nd_solution.hpp"
#include <map>

#pragma once

class phys_q
{
    std::map<phys_q_type, double> data;

public:
    phys_q()
    {
        for (int i = 0; i < (int)phys_q_type::COUNT; i++)
        {
            data[phys_q_type(i)] = 0.;
        }
    }

    std::map<phys_q_type, double> &get_modifiable_q()
    {
        return data;
    }

    void construct_res_for_link(unsigned int link_num, nd_solution *solution)
    {
        data[phys_q_type::pressure] = solution->get_pressure(link_num);
    }

    void construct_res_for_object(unsigned int link_num, nd_solution *solution)
    {
        data[phys_q_type::pressure] = solution->get_pressure(link_num);
        data[phys_q_type::water_molar_rate] = solution->get_wrat(link_num);
        if (solution->use_temperature)
            data[phys_q_type::temperature] = solution->get_temperature(link_num);
    }

    double &operator[](phys_q_type q)
    {
        return data[q];
    }
};

