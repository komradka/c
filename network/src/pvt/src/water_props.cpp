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
    case density_correlation::EOS:
        return "By EOS";
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
    case water_calculated_props::enthalpy:
        return "Enthalpy";

    case water_calculated_props::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(enthalpy_correlation e)
{
    switch (e)
    {
        case enthalpy_correlation::BO_correlation:
            return "Correlation";
        case enthalpy_correlation::EOS:
            return "By EOS";

        case enthalpy_correlation::COUNT:
            return "";
    }
    return "";
}

std::string get_name(viscosity_correlation e)
{
    switch (e)
    {
    case viscosity_correlation::none:
        return "NON";
    case viscosity_correlation::Grabovski:
        return "GRABOVSKI";
    case viscosity_correlation::Kestin:
        return "KESTIN";
    case viscosity_correlation::COUNT:
        return "";
    }
    return "";
}

std::string get_name(density_correlation e)
{
    switch (e)
    {
    case density_correlation::none:
        return "NON";
    case density_correlation::ROWE_CHOU:
        return "RC";
    case density_correlation::EOS:
        return "EOS";
    case density_correlation::COUNT:
        return "";
    }
    return "";
}

std::string get_name(enthalpy_correlation e)
{
    switch (e)
    {
        case enthalpy_correlation::BO_correlation:
            return "BO";
        case enthalpy_correlation::EOS:
            return "EOS";

        case enthalpy_correlation::COUNT:
            return "";
    }
    return "";
}

viscosity_correlation get_viscosity_correlation_by_string (std::string e)
{
    if (e == "NON")
        return viscosity_correlation::none;
    if (e == "GRABOVSKI")
        return viscosity_correlation::Grabovski;
    if (e == "KESTIN")
        return viscosity_correlation::Kestin;

    return viscosity_correlation::COUNT;
}
density_correlation get_density_correlation_by_string (std::string e)
{
    if (e == "NON")
        return density_correlation::none;
    if (e == "RC")
        return density_correlation::ROWE_CHOU;
    if (e == "EOS")
        return density_correlation::EOS;

    return density_correlation::COUNT;
}
enthalpy_correlation get_enthalpy_correlation_by_string (std::string e)
{
    if (e == "BO")
        return enthalpy_correlation::BO_correlation;
    if (e == "EOS")
        return enthalpy_correlation::EOS;

    return enthalpy_correlation::COUNT;
}
