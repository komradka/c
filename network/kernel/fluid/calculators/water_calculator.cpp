#include "water_calculator.hpp"
#include "../water_props.hpp"
#include "../../internal_constant.hpp"
#include <math.h>

#include <iostream>

error water_calculator::compute_water_density()
{
    RETURN_IF_FAIL(compute_1_B());

    double density = es_rc->density_sc * es_rc->FVF_1;

    es_rc->density = density;

    return error(OK);
}

error water_calculator::compute_1_B()
{
    double c = es_rc->compressibility;
    double x = c * (es_rc->pressure - es_rc->reference_pressure);

    double B = (1 + x + 0.5 * x * x) / es_rc->ref_FVF;

    es_rc->FVF_1 = B;

    return error(OK);
}

error water_calculator::compute_water_viscosity()
{
    double B = es_rc->FVF_1;

    double c = es_rc->compressibility;
    double cv = es_rc->viscosibility;

    double y = (c - cv) * (es_rc->pressure - es_rc->reference_pressure);

    double visc_1 = (1 + y + 0.5 * y * y) / B / es_rc->ref_viscosity / es_rc->ref_FVF;

    es_rc->viscosity = 1 / visc_1;
    return error(OK);
}

error water_calculator::compute_water_density_and_viscosity(density_correlation rho_corr,
                                                            viscosity_correlation v_corr)
{
    switch (rho_corr)
    {
    case density_correlation::none:
        RETURN_IF_FAIL(compute_water_density());
        break;
    case density_correlation::ROWE_CHOU:
        RETURN_IF_FAIL(compute_water_density_by_ROWE_CHOU());
        break;
    case density_correlation::COUNT:
        break;
    }

    switch (v_corr)
    {
    case viscosity_correlation::Grabovski:
        RETURN_IF_FAIL(compute_water_viscosity_by_Grabovski());
        break;
    case viscosity_correlation::Kestin:
        RETURN_IF_FAIL(compute_water_viscosity_by_Kestin());
        break;
    case viscosity_correlation::none:
        RETURN_IF_FAIL(compute_water_viscosity());
        break;
    case viscosity_correlation::COUNT:
        break;
    }

    return error(OK);
}

error water_calculator::compute_mass_rate()
{
    double vol_rate_sc = es_rc->volume_rate_sc;

    double dens = es_rc->density_sc;
    es_rc->mass_rate = dens * vol_rate_sc;

    return error(OK);
}

error water_calculator::compute_volume_rate()
{
    RETURN_IF_FAIL(compute_mass_rate());

    RETURN_IF_FAIL(compute_water_density());

    es_rc->volume_rate_rc = es_rc->mass_rate / es_rc->density;

    return error(OK);
}

error water_calculator::compute_water_viscosity_by_Grabovski()
{
    double temp_in_far = (es_rc->temperature + constant::ABS_ZERO_IN_CELSIUS) * 9 / 5 + 32;

    double A = 2.185;
    double B = 0.04012;
    double C = 5.1547e-6;

    es_rc->viscosity = A / (-1 + B * temp_in_far + C * temp_in_far * temp_in_far);

    return error(OK);
}

error water_calculator::compute_water_viscosity_by_Kestin()
{
    double p = es_rc->pressure * 0.1;
    double t = es_rc->temperature + constant::ABS_ZERO_IN_CELSIUS;

    double m = 0;

    double a1 = 3.324e-2, a2 = 3.624e-3, a3 = -1.879e-4;

    double b1 = -3.960e-2, b2 = 1.02e-2, b3 = -7.02e-4;

    double c1 = 1.2378, c2 = -1.303e-3, c3 = 3.06e-6, c4 = 2.55e-8;

    double betta_z_1 = 2.5, betta_z_2 = -2, betta_z_3 = 0.5;

    double betta_e_1 = 0.545, betta_e_2 = 2.8e-3;

    double betta_1 = -1.297, betta_2 = 5.74e-2, betta_3 = -6.97e-4, betta_4 = 4.47e-6, betta_5 = -1.05e-8;

    double m_0 = 6.044, m_1 = 2.8e-3, m_2 = 3.6e-5;

    double mu_z_20 = 1.002;

    double betta_w = betta_1 + betta_2 * t + betta_3 * t * t + betta_4 * t * t * t + betta_5 * t * t * t * t;
    double betta_e = betta_e_1 + betta_e_2 * t - betta_w;
    double m_s = m_0 + m_1 * t + m_2 * t * t;
    double s = m / m_s;
    double betta_z = betta_z_1 * s + betta_z_2 * s * s + betta_z_3 * s * s * s;
    double betta = betta_e * betta_z + betta_w;

    double dt = 20 - t;

    double nom = c1 * dt + c2 * dt * dt + c3 * dt * dt * dt + c4 * dt * dt * dt * dt;
    double denom = 96 + t;

    double f = nom / denom;

    double A = a1 * m + a2 * m * m + a3 * m * m * m;
    double B = b1 * m + b2 * m * m + b3 * m * m * m;

    double mu_z = mu_z_20 * pow(10, A + f * (B + 1));

    es_rc->viscosity = mu_z * (1 + 0.001 * betta * p);

    return error(OK);
}

error water_calculator::compute_water_density_by_ROWE_CHOU()
{
    double t = es_rc->temperature;
    double p = es_rc->pressure * 1.0197162129779;
    double m = 0;

    double A = 5.916365 - 0.01035794 * t + 0.9270048e-5 * t * t - 1127.522 / t + 100674.1 / t / t;
    double B = 0.5204914e-2 - 0.10482101e-4 * t + 0.8328532e-8 * t * t - 1.1702939 / t + 102.2783 / t / t;
    double C = 0.118547e-7 - 0.6599143e-10 * t;
    double D = -2.5166 + 0.0111766 * t - 0.170552e-4 * t * t;
    double E = 2.84851 - 0.0154305 * t + 0.223982e-4 * t * t;
    double F = -0.0014814 + 0.82969e-5 * t - 0.12469e-7 * t * t;
    double G = 0.0027141 - 0.15391e-4 * t + 0.22655e-7 * t * t;
    double H = 0.62158e-6 - 0.40075e-8 * t + 0.65972e-11 * t * t;

    double v = A - B * p - C * p * p + D * m + E * m * m - F * p * m - G * p * m * m - 0.5 * H * p * p * m;

    es_rc->density = 1000 / v;

    return error(OK);
}

error water_calculator::compute_enthalpy()
{
    double specific_density = es_rc->density_sc / constant::water_density_sc;

    es_rc->enthalpy = t_props->water_heat_capacity * (es_rc->temperature - 255.37) + (8.786214e-5 / specific_density) * es_rc->pressure;

    return error(OK);
}