#include "water_props.hpp"

#pragma once

struct fluid_cache
{
    bool is_already_computed(double mass_rate, double pressure, double temperature, condition cond)
    {
        if (this->mass_rate == mass_rate &&
            this->pressure == pressure &&
            this->temperature == temperature &&
            this->cond == cond)
            return true;
        return false;
    }

    void clear()
    {
        mass_rate = 0;
        pressure = 0;
        temperature = 0;
        density = 0;
        viscosity = 0;
    }

    void init_by_element_status(element_status *es, condition cond)
    {
        this->cond = cond;

        mass_rate = es->mass_rate;
        volume_rate_rc = es->volume_rate_rc;
        volume_rate_sc = es->volume_rate_sc;

        pressure = es->pressure;
        temperature = es->temperature;

        density = es->density;
        viscosity = es->viscosity;
    }

    condition cond;
    double mass_rate;
    double pressure;
    double temperature;

    double density;
    double viscosity;

    double volume_rate_rc;
    double volume_rate_sc;
};
