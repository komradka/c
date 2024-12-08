#include "object_data.hpp"
#include "../../error.hpp"

#include "nd_mixin.h"
#include "kernel/vertex.h"

#pragma once

class joint_data : public object_data
{
public:
    static constexpr int joint_fields = 2;

    static constexpr std::array<network_object_param, joint_fields> joint_params = {network_object_param::name,
                                                                                    network_object_param::activity};

public:
    joint_data()
    {
        using p = network_object_param;
        params[network_object_param::name] = std::nullopt;
        params[network_object_param::activity] = network_object_param_desc<p::activity>::default_type;
    }

    joint_data(joint_data &rhs) = default;

    joint_data &operator=(joint_data &rhs) = default;

    void write_data(std::ofstream &stream) override;

    bool add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count) override
    {
        (void)inlet_links_count;
        (void)outlet_links_count;
        return true;
    };
};

class nd_joint_data : public nd_mixin<joint_data, vertex>
{
public:
  network_objects get_object_type () const override { return network_objects::joint; }
};
