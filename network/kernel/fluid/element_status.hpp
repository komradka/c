
#pragma once

struct element_status
{
    double reference_pressure;

    double pressure;
    double temperature;

    double volume_rate_sc;
    double volume_rate_rc;
    double mass_rate;

    double density_sc;
    double density;

    double ref_viscosity;
    double viscosity;

    double ref_FVF;
    double FVF_1;
    
    double compressibility;
    double viscosibility;

    double enthalpy;
};