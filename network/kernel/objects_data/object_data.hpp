#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <any>
#include <array>

#include "object_params.hpp"
#include "../../utility/utility.hpp"
#include "../../error.hpp"

#pragma once

template <network_object_param Param>
struct network_object_param_desc;

class object_data
{
public:
    std::map<network_object_param, std::any> params;

public:
    virtual void write_data(std::ofstream &stream) = 0;
    virtual bool add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count) = 0;

    template <network_object_param Param>
    auto get_param() -> typename network_object_param_desc<Param>::data_type_t
    {
        using data_type_t = typename network_object_param_desc<Param>::data_type_t;

        auto value = params[Param];

        if (!value.has_value())
        {
            abort();
        }

        data_type_t p_value;

        try
        {
            p_value = std::any_cast<data_type_t>(value);
        }
        catch (const std::exception &e)
        {
            if constexpr (std::is_enum_v<data_type_t> == true)
            {
                p_value = (data_type_t)(0);
            }
            else
            {
                p_value = std::nullopt;
            }
        }

        return p_value;
    }

    template <network_object_param Param>
    void set_param(std::any value)
    {

        auto v = params[Param];

        if (!v.has_value())
        {
            abort();
        }

        params.at(Param) = value;
    }

    std::string get_name()
    {
        auto value = params[network_object_param::name];
        if (!value.has_value())
        {
            abort();
        }

        std::optional<std::string> opt;

        try
        {
            opt = std::any_cast<std::optional<std::string>>(value);
        }
        catch (const std::exception &e)
        {
            opt = std::nullopt;
        }

        return opt.has_value() ? opt.value() : "";
    }

    void set_name(std::optional<std::string> name)
    {
        params[network_object_param::name] = name;
    }

    bool is_active()
    {
        auto value = params[network_object_param::activity];
        if (!value.has_value())
        {
            abort();
        }

        std::optional<bool> opt = std::any_cast<std::optional<bool>>(value);

        return opt.value();
    }
};

std::string make_object_error(const std::string &err_message, const int line);
error set_pressure(const std::vector<std::string> &words, const int line, object_data *data);
error set_temperature(const std::vector<std::string> &words, const int line, object_data *data);
error set_wrat(const std::vector<std::string> &words, const int line, object_data *data);
error set_orat(const std::vector<std::string> &words, const int line, object_data *data);
error set_grat(const std::vector<std::string> &words, const int line, object_data *data);
error set_activity(const std::vector<std::string> &words, const int line, object_data *data);
error set_name(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_type(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_len(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_pressure_drop_coef(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_temp_drop_coef(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_height_diff(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_inner_diameter(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_roughness(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_wall_thickness(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_wall_cond(const std::vector<std::string> &words, const int line, object_data *data);
error set_pipe_burial_depth(const std::vector<std::string> &words, const int line, object_data *data);
error set_ambient_temperature(const std::vector<std::string> &words, const int line, object_data *data);
error set_htc(const std::vector<std::string> &words, const int line, object_data *data);