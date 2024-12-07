#include "../topology.hpp"
#include "../../reporter.hpp"
#include "../../manager_gui/settings_dialog.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <memory>

#pragma once

class nd_solution
{
private:
    reporter *rep;
    graph *network_topology;
    settings_dialog *settings;

    std::map<link_id, unsigned int> link_continuous_numeration;

    std::vector<unsigned int> pressure_eq_pos;
    std::vector<unsigned int> rate_eq_pos;
    std::vector<unsigned int> temp_eq_pos;

    unsigned int link_variables_count;

    std::unique_ptr<double[]> variables;

public:
    bool use_temperature = false;

public:
    nd_solution(reporter *_rep, graph *_network_topology, settings_dialog *_settings);

    unsigned int get_link_num(link_id);

    double get_pressure(unsigned int link_num);
    double get_wrat(unsigned int link_num);
    double get_temperature(unsigned int link_num);

    void set_pressure(unsigned int link_num, double p);
    void set_wrat(unsigned int link_num, double wrat);
    void set_temperature(unsigned int link_num, double t);

    unsigned int get_pressure_eq_pos(unsigned int link);
    unsigned int get_rate_eq_pos(unsigned int link);
    unsigned int get_temp_eq_pos(unsigned int link);

    unsigned int get_main_vars_count()
    {
        return link_variables_count * link_continuous_numeration.size();
    }

private:
    int get_pressure_pos()
    {
        return 0;
    }
    int get_wrat_pos() { return 1; }
    int get_temp_pos() { return 2; }
};