#include "object_data.hpp"
#include "../../error.hpp"

#pragma once

class source_data : public object_data
{
public:
    std::optional<double> pressure;
    std::optional<double> temperature;
    std::optional<double> water_rate;
    std::optional<double> oil_rate;
    std::optional<double> gas_rate;
    
    
    source_data() = default;

    source_data(std::optional<double> p,
                std::optional<double> t,
                std::optional<double> wrat,
                std::optional<double> orat,
                std::optional<double> grat,
                bool _activity,
                std::string _name)
    {
        pressure = p;
        temperature = t;
        water_rate = wrat;
        oil_rate = orat;
        gas_rate = grat;
        activity = _activity;
        name = _name;
    }

    source_data(source_data &rhs) = default;

    source_data& operator=(source_data &rhs) = default;
    
};

std::string make_source_error(const std::string &err_message, const int line);

error source_set_pressure(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_temperature(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_wrat(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_orat(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_grat(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_activity(const std::vector<std::string> &words, const int line, source_data &data);
error source_set_name(const std::vector<std::string> &words, const int line, source_data &data);
