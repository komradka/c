#include "nd_solution.hpp"

nd_solution::nd_solution(reporter *_rep, graph *_network_topology, settings_dialog *_settings)
{
    rep = _rep;
    network_topology = _network_topology;
    settings = _settings;

    use_temperature = _settings->get_param<solver_settings::use_temperature_equation>();

    link_variables_count = 2 + use_temperature ? 1 : 0;

    std::vector<link_id> links = network_topology->get_links();

    unsigned int links_count = links.size();

    for (unsigned int i = 0; i < links_count; i++)
    {
        link_continuous_numeration[links[i]] = i;
    }

    variables = std::make_unique<double[]>(links_count * link_variables_count);
}

unsigned int nd_solution::get_link_num(link_id link)
{
    auto it = link_continuous_numeration.find(link);
    if (it != link_continuous_numeration.end())
    {
        return it->second;
    }

    return -1;
}

double nd_solution::get_pressure(unsigned int link_num)
{
    return variables[link_num * link_variables_count + get_pressure_pos()];
}

double nd_solution::get_wrat(unsigned int link_num)
{
    return variables[link_num * link_variables_count + get_wrat_pos()];
}

double nd_solution::get_temperature(unsigned int link_num)
{
    return variables[link_num * link_variables_count + get_temp_pos()];
}

void nd_solution::set_pressure(unsigned int link_num, double p)
{
    variables[link_num * link_variables_count + get_pressure_pos()] = p;
}

void nd_solution::set_wrat(unsigned int link_num, double wrat)
{
    variables[link_num * link_variables_count + get_wrat_pos()] = wrat;
}

void nd_solution::set_temperature(unsigned int link_num, double t)
{
    variables[link_num * link_variables_count + get_temp_pos()] = t;
}