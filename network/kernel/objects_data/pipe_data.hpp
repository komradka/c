#include "object_data.hpp"
#include "../../error.hpp"

#include "nd_mixin.h"
#include "kernel/vertex.h"

#pragma once

class pipe_data : public object_data
{
public:
    static constexpr int pipe_fields = 14;

    static constexpr std::array<network_object_param, pipe_fields> pipe_params = {network_object_param::name,
                                                                                  network_object_param::activity,
                                                                                  network_object_param::pipe_type,
                                                                                  network_object_param::pipe_len,
                                                                                  network_object_param::pipe_pressure_drop_coef,
                                                                                  network_object_param::pipe_temp_drop_coef,
                                                                                  network_object_param::pipe_height_diff,
                                                                                  network_object_param::pipe_inner_diameter,
                                                                                  network_object_param::pipe_roughness,
                                                                                  network_object_param::pipe_burial_depth,
                                                                                  network_object_param::pipe_wall_thickness,
                                                                                  network_object_param::pipe_wall_cond,
                                                                                  network_object_param::ambient_temperature,
                                                                                  network_object_param::htc};

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
        params[network_object_param::pipe_height_diff] = network_object_param_desc<p::pipe_height_diff>::default_type;
        params[network_object_param::pipe_inner_diameter] = network_object_param_desc<p::pipe_inner_diameter>::default_type;
        params[network_object_param::pipe_roughness] = network_object_param_desc<p::pipe_roughness>::default_type;
        params[network_object_param::pipe_burial_depth] = network_object_param_desc<p::pipe_burial_depth>::default_type;
        params[network_object_param::pipe_wall_thickness] = network_object_param_desc<p::pipe_wall_thickness>::default_type;
        params[network_object_param::pipe_wall_cond] = network_object_param_desc<p::pipe_wall_cond>::default_type;
        params[network_object_param::ambient_temperature] = network_object_param_desc<p::ambient_temperature>::default_type;
        params[network_object_param::htc] = network_object_param_desc<p::htc>::default_type;
    }

    pipe_data(pipe_data &rhs) = default;

    pipe_data &operator=(pipe_data &rhs) = default;

    void write_data(std::ofstream &stream) override;

    bool add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count) override;
};

class nd_pipe_data : public nd_mixin<pipe_data, vertex>
{
public:
  network_objects get_object_type () const override { return network_objects::pipe; }
};
