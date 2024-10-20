#include "sink_data.hpp"

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

    data.pressure = pressure;
    return error(OK);
}

error sink_set_temperature(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double temp;

    if (!sscanf(words[2].c_str(), "%lf", &temp))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.temperature = temp;
    return error(OK);
}

error sink_set_wrat(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double wrat;

    if (!sscanf(words[2].c_str(), "%lf", &wrat))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.water_rate = wrat;
    return error(OK);
}

error sink_set_orat(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double orat;

    if (!sscanf(words[2].c_str(), "%lf", &orat))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.oil_rate = orat;
    return error(OK);
}

error sink_set_grat(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    double grat;

    if (!sscanf(words[2].c_str(), "%lf", &grat))
        return error(make_sink_error("Wrong word: " + words[2], line));

    data.gas_rate = grat;
    return error(OK);
}

error sink_set_activity(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    std::string act = words[2];

    if (act == "ACTIVE")
        data.activity = true;
    else if (act == "INACTIVE")
        data.activity = false;
    else
        return error(make_sink_error("Wrong word: ", line));

    return error(OK);
}

error sink_set_name(const std::vector<std::string> &words, const int line, sink_data &data)
{
    if (words.size() != 3)
        return error(make_sink_error("Wrong number of fields", line));
    data.name = words[2];
    return error(OK);
}
