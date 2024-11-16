#include "object_data.hpp"
#include "../../error.hpp"

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

std::string make_source_error(const std::string &err_message, const int line);

error source_set_pressure(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_temperature(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_wrat(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_orat(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_grat(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_activity(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_name(const std::vector<std::string> &words, const int line, source_data &data);
