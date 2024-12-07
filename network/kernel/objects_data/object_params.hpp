#include <optional>
#include <string>
#include "object_param_enum.hpp"

#pragma once

enum class network_object_param
{
    name,
    activity,
    pressure,
    temperature,
    wrat,
    orat,
    grat,

    pipe_type,
    pipe_len,
    pipe_pressure_drop_coef,
    pipe_temp_drop_coef,
    pipe_height_diff,
    pipe_inner_diameter,
    pipe_roughness,
    pipe_wall_thickness,
    pipe_wall_cond,
    pipe_burial_depth,
    ambient_temperature,
    htc,

    COUNT,
};

template <network_object_param Param>
struct network_object_param_desc;

template <>
struct network_object_param_desc<network_object_param::name>
{
    using data_type_t = std::optional<std::string>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "NAME";
    static constexpr auto name_for_gui = "Object Name";
};

template <>
struct network_object_param_desc<network_object_param::activity>
{
    using data_type_t = std::optional<bool>;
    static constexpr auto default_type = std::make_optional(true);
    static constexpr auto name = "ACTIVITY";
    static constexpr auto name_for_gui = "Activity";
};

template <>
struct network_object_param_desc<network_object_param::pressure>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "PRESSURE";
    static constexpr auto name_for_gui = "Pressure";
};

template <>
struct network_object_param_desc<network_object_param::temperature>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "TEMPERATURE";
    static constexpr auto name_for_gui = "Temperature";
};

template <>
struct network_object_param_desc<network_object_param::wrat>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "WRAT";
    static constexpr auto name_for_gui = "Water Rate";
};

template <>
struct network_object_param_desc<network_object_param::orat>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "ORAT";
    static constexpr auto name_for_gui = "Oil Rate";
};

template <>
struct network_object_param_desc<network_object_param::grat>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "GRAT";
    static constexpr auto name_for_gui = "Gas Rate";
};

template <>
struct network_object_param_desc<network_object_param::pipe_type>
{
    using data_type_t = pipe_type;
    static constexpr auto default_type = pipe_type::constant_coef;
    static constexpr auto name = "PIPE_TYPE";
    static constexpr auto name_for_gui = "Pipe calculation type";
};

template <>
struct network_object_param_desc<network_object_param::pipe_len>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(1000.);
    static constexpr auto name = "PIPE_LEN";
    static constexpr auto name_for_gui = "Pipe length";
};

template <>
struct network_object_param_desc<network_object_param::pipe_pressure_drop_coef>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.1);
    static constexpr auto name = "PIPE_P_COEF";
    static constexpr auto name_for_gui = "Pipe pressure drop coefficient";
};

template <>
struct network_object_param_desc<network_object_param::pipe_temp_drop_coef>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.1);
    static constexpr auto name = "PIPE_T_COEF";
    static constexpr auto name_for_gui = "Pipe temperature drop coefficient";
};

template <>
struct network_object_param_desc<network_object_param::pipe_height_diff>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.);
    static constexpr auto name = "PIPE_H_DIFF";
    static constexpr auto name_for_gui = "Height difference";
};

template <>
struct network_object_param_desc<network_object_param::pipe_inner_diameter>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.2);
    static constexpr auto name = "PIPE_D_IN";
    static constexpr auto name_for_gui = "Inner Diameter";
};

template <>
struct network_object_param_desc<network_object_param::pipe_roughness>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.01);
    static constexpr auto name = "PIPE_ROUGH";
    static constexpr auto name_for_gui = "Roughness";
};

template <>
struct network_object_param_desc<network_object_param::pipe_burial_depth>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.);
    static constexpr auto name = "PIPE_DEPTH";
    static constexpr auto name_for_gui = "Burial Depth";
};

template <>
struct network_object_param_desc<network_object_param::pipe_wall_cond>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(40.);
    static constexpr auto name = "PIPE_COND";
    static constexpr auto name_for_gui = "Wall Conductivity";
};

template <>
struct network_object_param_desc<network_object_param::pipe_wall_thickness>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(0.015);
    static constexpr auto name = "PIPE_THK";
    static constexpr auto name_for_gui = "Wall Thickness";
};

template <>
struct network_object_param_desc<network_object_param::ambient_temperature>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::make_optional(15.56);
    static constexpr auto name = "PIPE_TA";
    static constexpr auto name_for_gui = "Ambient Temperature";
};

template <>
struct network_object_param_desc<network_object_param::htc>
{
    using data_type_t = std::optional<double>;
    static constexpr auto default_type = std::nullopt;
    static constexpr auto name = "PIPE_HTC";
    static constexpr auto name_for_gui = "Heat Transfer Coefficient";
};
