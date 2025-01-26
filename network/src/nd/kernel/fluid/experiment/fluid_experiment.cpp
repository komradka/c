#include "fluid_experiment.hpp"
#include "../../internal_constant.hpp"

void fluid_experiment::fill_element_status(double volume_rate_sc,
                                           double pressure,
                                           double temperature,
                                           element_status *es)
{
    es->volume_rate_sc = volume_rate_sc;
    es->pressure = pressure;
    es->temperature = temperature - constant::ABS_ZERO_IN_CELSIUS;
}

error fluid_experiment::start_experiment(double &result)
{
    fluid->compute_all_props(condition::rc);

    fluid_cache cache = fluid->get_cache(condition::rc);

    switch (need_calculate)
    {
    case water_calculated_props::density:
        result = cache.density;
        break;
    case water_calculated_props::viscosity:
        result = cache.viscosity;
        break;
    case water_calculated_props::mass_rate:
        result = cache.mass_rate;
        break;
    case water_calculated_props::volume_rate_rc:
        result = cache.volume_rate_rc;
        break;
    case water_calculated_props::COUNT:
        break;
    }

    return error(OK);
}

error fluid_experiment::start_experiment(double volume_rate_sc,
                                         double pressure,
                                         double temperature,
                                         double &res)
{
    fill_element_status(volume_rate_sc, pressure, temperature, fluid->get_element_status(condition::rc));

    start_experiment(res);
    return error(OK);
}