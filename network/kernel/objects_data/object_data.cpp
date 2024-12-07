#include "object_data.hpp"

std::string make_object_error(const std::string &err_message, const int line)
{
    return "Wrong data file format: " + err_message + ".\n" + "In line - " + std::to_string(line);
}

error set_pressure(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double pressure;

    if (!sscanf(words[2].c_str(), "%lf", &pressure))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pressure] = std::make_optional<double>(pressure);

    return error(OK);
}

error set_temperature(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double temp;

    if (!sscanf(words[2].c_str(), "%lf", &temp))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::temperature] = std::make_optional<double>(temp);

    return error(OK);
}

error set_wrat(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double wrat;

    if (!sscanf(words[2].c_str(), "%lf", &wrat))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::wrat] = std::make_optional<double>(wrat);

    return error(OK);
}

error set_orat(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double orat;

    if (!sscanf(words[2].c_str(), "%lf", &orat))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::orat] = std::make_optional<double>(orat);

    return error(OK);
}

error set_grat(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double grat;

    if (!sscanf(words[2].c_str(), "%lf", &grat))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::grat] = std::make_optional<double>(grat);

    return error(OK);
}

error set_activity(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    std::string act = words[2];

    if (act == "ACTIVE" || act == "1")
    {
        data->params[network_object_param::activity] = std::make_optional<bool>(true);
    }
    else if (act == "INACTIVE" || act == "0")
    {
        data->params[network_object_param::activity] = std::make_optional<bool>(false);
    }
    else
        return error(make_object_error("Wrong word: ", line));

    return error(OK);
}

error set_name(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() < 3)
        return error(make_object_error("Wrong number of fields", line));

    std::string name = "";
    for (int i = 3; i < isize (words); i++)
    {
        name += words[i];
        if (i != isize (words) - 1)
        {
            name += "_";
        }
    }

    data->params[network_object_param::name] = std::make_optional<std::string>(name);
    return error(OK);
}

error set_pipe_type(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));

    pipe_type type = get_pipe_type_by_storage_name(words[2]);
    if (type == pipe_type::COUNT)
        return error(make_object_error("Wrong word: ", line));

    data->params[network_object_param::pipe_type] = type;
    return error(OK);
}

error set_pipe_len(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double len;

    if (!sscanf(words[2].c_str(), "%lf", &len))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_len] = std::make_optional<double>(len);

    return error(OK);
}

error set_pipe_pressure_drop_coef(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double coef;

    if (!sscanf(words[2].c_str(), "%lf", &coef))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_pressure_drop_coef] = std::make_optional<double>(coef);

    return error(OK);
}

error set_pipe_temp_drop_coef(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double coef;

    if (!sscanf(words[2].c_str(), "%lf", &coef))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_temp_drop_coef] = std::make_optional<double>(coef);

    return error(OK);
}

error set_pipe_height_diff(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double dh;

    if (!sscanf(words[2].c_str(), "%lf", &dh))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_height_diff] = std::make_optional<double>(dh);

    return error(OK);
}

error set_pipe_inner_diameter(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double d;

    if (!sscanf(words[2].c_str(), "%lf", &d))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_inner_diameter] = std::make_optional<double>(d);

    return error(OK);
}

error set_pipe_roughness(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double roughness;

    if (!sscanf(words[2].c_str(), "%lf", &roughness))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_roughness] = std::make_optional<double>(roughness);

    return error(OK);
}

error set_pipe_wall_thickness(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double thickness;

    if (!sscanf(words[2].c_str(), "%lf", &thickness))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_wall_thickness] = std::make_optional<double>(thickness);

    return error(OK);
}

error set_pipe_wall_cond(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double cond;

    if (!sscanf(words[2].c_str(), "%lf", &cond))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_wall_cond] = std::make_optional<double>(cond);

    return error(OK);
}

error set_pipe_burial_depth(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double depth;

    if (!sscanf(words[2].c_str(), "%lf", &depth))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::pipe_burial_depth] = std::make_optional<double>(depth);

    return error(OK);
}

error set_ambient_temperature(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double temp;

    if (!sscanf(words[2].c_str(), "%lf", &temp))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::ambient_temperature] = std::make_optional<double>(temp);

    return error(OK);
}

error set_htc(const std::vector<std::string> &words, const int line, object_data *data)
{
    if (words.size() != 3)
        return error(make_object_error("Wrong number of fields", line));
    double htc;

    if (!sscanf(words[2].c_str(), "%lf", &htc))
        return error(make_object_error("Wrong word: " + words[2], line));

    data->params[network_object_param::htc] = std::make_optional<double>(htc);

    return error(OK);
}
