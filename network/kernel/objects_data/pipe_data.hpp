#include "object_data.hpp"
#include "../../error.hpp"

#pragma once

class pipe_data : public object_data
{
public:
    static constexpr int pipe_fields = 6;

    static constexpr std::array<network_object_param, pipe_fields> pipe_params = {network_object_param::name,
                                                                                  network_object_param::activity,
                                                                                  network_object_param::pipe_type,
                                                                                  network_object_param::pipe_len,
                                                                                  network_object_param::pipe_pressure_drop_coef,
                                                                                  network_object_param::pipe_temp_drop_coef};

    static constexpr unsigned int inlet_links_count = 1;
    static constexpr unsigned int outlet_links_count = 1;

public:
    pipe_data()
    {
        using p = network_object_param;
        params[network_object_param::name] = std::nullopt;
        params[network_object_param::activity] = network_object_param_desc<p::activity>::default_type;
        params[network_object_param::pipe_type] = network_object_param_desc<p::pipe_type>::default_type;
        params[network_object_param::pipe_len] = network_object_param_desc<p::pipe_len>::default_type;
        params[network_object_param::pipe_pressure_drop_coef] = network_object_param_desc<p::pipe_pressure_drop_coef>::default_type;
        params[network_object_param::pipe_temp_drop_coef] = network_object_param_desc<p::pipe_temp_drop_coef>::default_type;
    }

    pipe_data(pipe_data &rhs) = default;

    pipe_data &operator=(pipe_data &rhs) = default;

    void write_data(std::ofstream &stream) override;

    bool add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count) override;
};