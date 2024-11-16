#include "sink_data.hpp"

void sink_data::write_data(std::ofstream &stream)
{
    auto write_param = [&](auto i)
    {
        using data_type_t = typename network_object_param_desc<sink_data::sink_params[i]>::data_type_t;
        data_type_t value = get_param<sink_data::sink_params[i]>();

        if (value.has_value())
            stream << "SET " << network_object_param_desc<sink_data::sink_params[i]>::name << " " << value.value() << std::endl;
    };

    constexpr_for<0, sink_data::sink_fields>(write_param);
}

std::string make_sink_error(const std::string &err_message, const int line)
{
    return "Wrong sink data file format: " + err_message + ".\n" + "In line - " + std::to_string(line);
}

error sink_set_pressure(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double pressure;

    if (!sscanf(words[2].c_str(), "%lf", &pressure))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.params[network_object_param::pressure] = std::make_optional<double>(pressure);

    // data.set_param<network_object_param::pressure>(std::make_optional<double>(pressure));

    return error(OK);
}

error sink_set_temperature(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double temp;

    if (!sscanf(words[2].c_str(), "%lf", &temp))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.params[network_object_param::temperature] = std::make_optional<double>(temp);

    // data.set_param<network_object_param::temperature>(std::make_optional<double>(temp));
    return error(OK);
}

error sink_set_wrat(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double wrat;

    if (!sscanf(words[2].c_str(), "%lf", &wrat))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.params[network_object_param::wrat] = std::make_optional<double>(wrat);

    // data.set_param<network_object_param::wrat>(std::make_optional<double>(wrat));
    return error(OK);
}

error sink_set_orat(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double orat;

    if (!sscanf(words[2].c_str(), "%lf", &orat))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.params[network_object_param::orat] = std::make_optional<double>(orat);

    // data.set_param<network_object_param::orat>(std::make_optional<double>(orat));
    return error(OK);
}

error sink_set_grat(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double grat;

    if (!sscanf(words[2].c_str(), "%lf", &grat))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.params[network_object_param::grat] = std::make_optional<double>(grat);
    // data.set_param<network_object_param::grat>(std::make_optional<double>(grat));
    return error(OK);
}

error sink_set_activity(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    std::string act = words[2];

    if (act == "ACTIVE" || act == "1")
    {
        data.params[network_object_param::activity] = std::make_optional<bool>(true);
        // data.set_param<network_object_param::activity>(true);
    }
    else if (act == "INACTIVE" || act == "0")
    {
        data.params[network_object_param::activity] = std::make_optional<bool>(false);
        // data.set_param<network_object_param::activity>(false);
    }
    else
        return error(make_sink_error("Wrong word: ", line));

    return error(OK);
}

error sink_set_name(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));

    data.params[network_object_param::name] = std::make_optional<std::string>(words[2]);

    // data.set_param<network_object_param::name>(std::make_optional<std::string>(words[2]));

    return error(OK);
}

bool sink_data::add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count)
{
    if (inlet_links_count > this->inlet_links_count || outlet_links_count > this->outlet_links_count)
        return false;
    return true;
}
