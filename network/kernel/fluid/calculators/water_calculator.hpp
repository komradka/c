#include "../../../error.hpp"
#include "../element_status.hpp"
#include "../thermal_props.hpp"

#pragma once

enum class density_correlation;
enum class viscosity_correlation;

class water_calculator
{
private:
    element_status *es_sc;
    element_status *es_rc;
    thermal_props *t_props;

public:
    water_calculator(element_status *es_sc, element_status *es_rc, thermal_props *t_props)
    {
        this->es_rc = es_rc;
        this->es_sc = es_sc;
        this->t_props = t_props;
    }
    ~water_calculator()
    {
        delete t_props;
    }

    error compute_water_density_and_viscosity(density_correlation rho_corr,
                                              viscosity_correlation v_corr);
    error compute_mass_rate();
    error compute_volume_rate();
    error compute_enthalpy();

private:
    error compute_water_density();
    error compute_water_density_by_ROWE_CHOU();
    error compute_water_viscosity();
    error compute_water_viscosity_by_Grabovski();
    error compute_water_viscosity_by_Kestin();
    error compute_1_B();
};