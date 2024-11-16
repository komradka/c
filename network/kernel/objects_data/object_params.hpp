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