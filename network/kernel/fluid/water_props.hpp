#include <string>

#pragma once

enum class water_props
{
    reference_pressure, // double
    FVF,                // double
    compressibility,    // double
    viscosity,          // double
    viscosibility,      // double
    density,            // double

    viscosity_correlation, // enum
    density_correlation,   // enum

    COUNT
};

enum class viscosity_correlation
{
    none,
    Grabovski,
    Kestin,

    COUNT
};

enum class density_correlation
{
    none,
    ROWE_CHOU,

    COUNT
};

enum class condition
{
    sc,
    rc,

    COUNT
};

std::string get_name_for_gui(viscosity_correlation e);
std::string get_name_for_gui(density_correlation e);

enum class water_calculated_props
{
    density,
    viscosity,

    mass_rate,
    volume_rate_rc,

    COUNT
};

std::string get_name_for_gui(water_calculated_props e);