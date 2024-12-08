#include "object_data.hpp"
#include "../../error.hpp"

#include "nd_mixin.h"
#include "kernel/vertex.h"

#pragma once

class sink_data : public object_data
{
public:
    static constexpr int sink_fields = 7;
    static constexpr std::array<network_object_param, sink_fields> sink_params = {network_object_param::name,
                                                                                  network_object_param::activity,
                                                                                  network_object_param::pressure,
                                                                                  network_object_param::temperature,
                                                                                  network_object_param::wrat,
                                                                                  network_object_param::orat,
                                                                                  network_object_param::grat};

    static constexpr unsigned int inlet_links_count = 1;
    static constexpr unsigned int outlet_links_count = 0;

public:
    sink_data()
    {
        params[network_object_param::name] = std::nullopt;
        params[network_object_param::activity] = std::make_optional<bool>(true);
        params[network_object_param::pressure] = std::nullopt;
        params[network_object_param::temperature] = std::nullopt;
        params[network_object_param::wrat] = std::nullopt;
        params[network_object_param::orat] = std::nullopt;
        params[network_object_param::grat] = std::nullopt;
    }

    sink_data(sink_data &rhs) = default;

    sink_data &operator=(sink_data &rhs) = default;

    void write_data(std::ofstream &stream) override;

    bool add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count) override;
};

class nd_sink_data : public nd_mixin<sink_data, vertex>
{
public:
  network_objects get_object_type () const override { return network_objects::sink; }
};
