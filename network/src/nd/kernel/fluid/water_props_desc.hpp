#include <string>

#include "water_props.hpp"
#include "../internal_constant.hpp"

#pragma once

template <water_props Props>
struct water_props_desc;

template <>
struct water_props_desc<water_props::reference_pressure>
{
    using data_type_t = double;
    static constexpr auto default_type = constant::standard_pressure;
    static constexpr auto name = "reference_pressure";
    static constexpr auto name_for_gui = "Reference Pressure";
};

template <>
struct water_props_desc<water_props::FVF>
{
    using data_type_t = double;
    static constexpr auto default_type = 1.;
    static constexpr auto name = "reference_FVF";
    static constexpr auto name_for_gui = "Reference FVF";
};

template <>
struct water_props_desc<water_props::compressibility>
{
    using data_type_t = double;
    static constexpr auto default_type = 3.94e-05;
    static constexpr auto name = "compressibility";
    static constexpr auto name_for_gui = "Compressibility";
};

template <>
struct water_props_desc<water_props::viscosity>
{
    using data_type_t = double;
    static constexpr auto default_type = constant::water_viscosity_sc;
    static constexpr auto name = "reference_viscosity";
    static constexpr auto name_for_gui = "Reference Viscosity";
};

template <>
struct water_props_desc<water_props::viscosibility>
{
    using data_type_t = double;
    static constexpr auto default_type = 0.;
    static constexpr auto name = "viscosibility";
    static constexpr auto name_for_gui = "Viscosibility";
};

template <>
struct water_props_desc<water_props::density>
{
    using data_type_t = double;
    static constexpr auto default_type = constant::water_density_sc;
    static constexpr auto name = "water_density_sc";
    static constexpr auto name_for_gui = "Water SC density";
};

template <>
struct water_props_desc<water_props::density_correlation>
{
    using data_type_t = density_correlation;
    static constexpr auto default_type = density_correlation::none;
    static constexpr auto name = "water_density_correlation";
    static constexpr auto name_for_gui = "Water Density Correlation";
};

template <>
struct water_props_desc<water_props::viscosity_correlation>
{
    using data_type_t = viscosity_correlation;
    static constexpr auto default_type = viscosity_correlation::none;
    static constexpr auto name = "water_viscosity_correlation";
    static constexpr auto name_for_gui = "Water Viscosity Correlation";
};
