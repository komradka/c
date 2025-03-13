#include "calculators/water_calculator.hpp"
#include "fluid_widget.hpp"
#include "fluid_cache.hpp"

#pragma once

#define D_RATE 0.

class fluid_experiment;

class fluid_props
{
private:
    bool is_filled = false;

    water_calculator *w_calc;
    fluid_widget *fw;
    thermal_props *t_props;

    element_status *es_rc;
    element_status *es_sc;

    fluid_cache sc_cache;
    fluid_cache rc_cache;

public:
    friend class fluid_experiment;

    fluid_props ()
    {
        t_props = new thermal_props;

        es_sc = new element_status();
        es_rc = new element_status();
        w_calc = new water_calculator(es_sc, es_rc, t_props);

        sc_cache.clear();
        rc_cache.clear();
        sc_cache.cond = condition::sc;
        rc_cache.cond = condition::rc;
    }

    fluid_props(fluid_widget *fw)
    {
        this->fw = fw;

        t_props = new thermal_props;

        es_sc = new element_status();
        es_rc = new element_status();
        w_calc = new water_calculator(es_sc, es_rc, t_props);

        sc_cache.clear();
        rc_cache.clear();
        sc_cache.cond = condition::sc;
        rc_cache.cond = condition::rc;

        fill_element_status_by_props(es_rc);
        fill_element_status_by_props(es_rc);

        is_filled = true;
    }

    ~fluid_props()
    {
        delete es_sc;
        delete es_rc;
        delete w_calc;
    }

    element_status *get_element_status(condition cond)
    {
        if (cond == condition::sc)
            return es_sc;
        return es_rc;
    }

    fluid_cache &get_cache(condition cond)
    {
        if (cond == condition::sc)
            return sc_cache;
        return rc_cache;
    }

    void fill_element_status_by_props(element_status *es)
    {
        es->compressibility = fw->get_param<water_props::compressibility>();
        es->density_sc = fw->get_param<water_props::density>();
        es->ref_FVF = fw->get_param<water_props::FVF>();
        es->ref_viscosity = fw->get_param<water_props::viscosity>();
        es->reference_pressure = fw->get_param<water_props::reference_pressure>();
        es->viscosibility = fw->get_param<water_props::viscosibility>();
    }

private:
    error compute_all_props(double pressure, double temperature, double volume_rate_sc, condition cond)
    {
        if(!is_filled)
            abort();

        es_rc->pressure = pressure;
        es_rc->temperature = temperature;
        es_rc->volume_rate_sc = volume_rate_sc;

        if (cond == condition::sc)
        {
            return error(OK);
        }
        else
        {
            RETURN_IF_FAIL(w_calc->run_flash(fw->get_param<water_props::density_correlation>(),
                                             fw->get_param<water_props::viscosity_correlation>(),
                                             fw->get_param<water_props::enthalpy_correlation>()));
        }

        get_cache(cond).init_by_element_status(get_element_status(cond), cond);
        return error(OK);
    }

public:
    error compute_density(double pressure, double temperature, double &density)
    {
        RETURN_IF_FAIL (compute_all_props (pressure, temperature, D_RATE, condition::rc));

        density = es_rc->density;

        return error(OK);
    }

    error compute_density_sc(double &density)
    {
        density = fw->get_param<water_props::density>();
        return error(OK);
    }

    error compute_viscosity(double pressure, double temperature, double &viscosity)
    {
        RETURN_IF_FAIL (compute_all_props (pressure, temperature, D_RATE, condition::rc));

        viscosity = es_rc->viscosity;

        return error(OK);
    }

    error compute_enthalpy(double temperature, double pressure, double &enthalpy)
    {
        RETURN_IF_FAIL (compute_all_props (pressure, temperature, D_RATE, condition::rc));

        enthalpy = es_rc->enthalpy;

        return error(OK);
    }

    double get_water_heat_capacity()
    {
        return t_props->water_heat_capacity;
    }

    double get_water_conductivity()
    {   
        return t_props->water_conductivity;
    }

public:
    void copy_from(fluid_props *rhs)
    {
        fw = new fluid_widget();
        fw->copy_from(rhs->get_widget());

        fill_element_status_by_props(es_rc);
        fill_element_status_by_props(es_sc);

        is_filled = true;
    }

    fluid_widget *get_widget()
    {
        return fw;
    }
};
