#include "object_data.hpp"

#include "nd_mixin.h"
#include "kernel/vertex.h"

#pragma once

class source_data : public object_data
{
public:
    static constexpr int source_fields = 7;
    static constexpr std::array<network_object_param, source_fields> source_params = {network_object_param::name,
                                                                                      network_object_param::activity,
                                                                                      network_object_param::pressure,
                                                                                      network_object_param::temperature,
                                                                                      network_object_param::wrat,
                                                                                      network_object_param::orat,
                                                                                      network_object_param::grat};

    static constexpr unsigned int inlet_links_count = 0;
    static constexpr unsigned int outlet_links_count = 1;

public:
    source_data()
    {
        params[network_object_param::name] = std::nullopt;
        params[network_object_param::activity] = std::make_optional<bool>(true);
        params[network_object_param::pressure] = std::nullopt;
        params[network_object_param::temperature] = std::nullopt;
        params[network_object_param::wrat] = std::nullopt;
        params[network_object_param::orat] = std::nullopt;
        params[network_object_param::grat] = std::nullopt;
    }

    source_data(source_data &rhs) = default;

    source_data &operator=(source_data &rhs) = default;

    void write_data(std::ofstream &stream) override;

    bool add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count) override;
};

class nd_source_data : public nd_mixin<source_data, vertex>
{
public:
  network_objects get_object_type () const override { return network_objects::source; }
};
