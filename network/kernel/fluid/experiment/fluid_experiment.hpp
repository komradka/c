#include "../fluid_props.hpp"

#pragma once

class fluid_experiment
{
private:
    fluid_props *fluid;

    water_calculated_props need_calculate;

public:
    fluid_experiment(double volume_rate_sc,
                     double pressure,
                     double temperature,
                     water_calculated_props res_type,
                     QWidget *fw)
    {
        fluid = new fluid_props(dynamic_cast<fluid_widget *>(fw));

        fill_element_status(volume_rate_sc,
                            pressure,
                            temperature,
                            fluid->get_element_status(condition::rc));

        this->need_calculate = res_type;
    }

    fluid_experiment(water_calculated_props res_type,
                     QWidget *fw)
    {
        fluid = new fluid_props(dynamic_cast<fluid_widget *>(fw));
        this->need_calculate = res_type;
    }

    ~fluid_experiment()
    {
        delete fluid;
    }

private:
    void fill_element_status(double volume_rate_sc,
                             double pressure,
                             double temperature,
                             element_status *es);

public:
    error start_experiment(double &res);

    error start_experiment(double volume_rate_sc,
                           double pressure,
                           double temperature,
                           double &res);
};