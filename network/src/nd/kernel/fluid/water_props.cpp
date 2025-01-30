#include "water_props.hpp"

std::string get_name_for_gui(viscosity_correlation e)
{
    switch (e)
    {
    case viscosity_correlation::none:
        return "None";
    case viscosity_correlation::Grabovski:
        return "Grabovski";
    case viscosity_correlation::Kestin:
        return "Kestin";
    case viscosity_correlation::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(density_correlation e)
{
    switch (e)
    {
    case density_correlation::none:
        return "None";
    case density_correlation::ROWE_CHOU:
        return "ROWE-CHOU method";
    case density_correlation::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(water_calculated_props e)
{
    switch (e)
    {
    case water_calculated_props::density:
        return "Density";
    case water_calculated_props::viscosity:
        return "Viscosity";
    case water_calculated_props::mass_rate:
        return "Mass Rate";
    case water_calculated_props::volume_rate_rc:
        return "Volume Rate RC";

    case water_calculated_props::COUNT:
        return "";
    }
    return "";
}