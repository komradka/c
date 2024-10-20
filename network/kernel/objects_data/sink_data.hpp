#include "object_data.hpp"
#include "../../error.hpp"

#pragma once

class sink_data : public object_data
{
public:
    std::optional<double> pressure;
    std::optional<double> temperature;
    std::optional<double> water_rate;
    std::optional<double> oil_rate;
    std::optional<double> gas_rate;

    sink_data() = default;

    sink_data(std::optional<double> p,
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

    sink_data(sink_data &rhs) = default;

    sink_data &operator=(sink_data &rhs) = default;
};

std::string make_sink_error(const std::string &err_message, const int line);

error sink_set_pressure(const std::vector<std::string> &words, const int line, sink_data &data);
error sink_set_temperature(const std::vector<std::string> &words, const int line, sink_data &data);
error sink_set_wrat(const std::vector<std::string> &words, const int line, sink_data &data);
error sink_set_orat(const std::vector<std::string> &words, const int line, sink_data &data);
error sink_set_grat(const std::vector<std::string> &words, const int line, sink_data &data);
error sink_set_activity(const std::vector<std::string> &words, const int line, sink_data &data);
error sink_set_name(const std::vector<std::string> &words, const int line, sink_data &data);
