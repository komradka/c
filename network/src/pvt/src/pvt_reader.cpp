#include "pvt_manager.hpp"
#include "format.hpp"
#include <sstream>

error set_ref_p (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_ref_fvf (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_compress (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_ref_visc (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_viscoss (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_w_rho (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_w_rho_corr (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_w_visc_corr (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_w_enth_corr (const std::vector<std::string> &words, const int line, std::vector<std::any> &res);

void init_pvt_key_words(std::map<std::string, int> &pvt_key_words)
{
    auto init = [&](auto i)
    {
        pvt_key_words[water_props_desc<fluid_widget::all_props[i]>::name] = i;
    };

    constexpr_for<0, fluid_widget::props_count>(init);
}

void init_pvt_func(std::map<int, std::function<error(const std::vector<std::string> &, const int, std::vector<std::any> &)>> &pvt_func)
{
    pvt_func[0] = set_ref_p;
    pvt_func[1] = set_ref_fvf;
    pvt_func[2] = set_compress;
    pvt_func[3] = set_ref_visc;
    pvt_func[4] = set_viscoss;
    pvt_func[5] = set_w_rho;
    pvt_func[6] = set_w_rho_corr;
    pvt_func[7] = set_w_visc_corr;
    pvt_func[8] = set_w_enth_corr;

    if (pvt_func.size() != fluid_widget::props_count)
        abort();
}

error pvt_manager::read_pvt_string (const std::string str, const int line, std::vector<std::any> &readed_props)
{
    std::vector<std::string> words;
    fmt::split_string(str, words);

    if (words.size() < 1)
    {
        return error("Wrong number of fields");
    }
    std::string main_word = words[0];

    if (pvt_key_words.count(main_word))
    {
        int index = pvt_key_words.at(main_word);
        RETURN_IF_FAIL(pvt_func.at(index)(words, line, readed_props));
        return error(OK);
    }

    return error("Unknown pvt word");
}

void pvt_manager::read_pvt(std::string path)
{
    make_fluid_widget();

    std::ifstream file;

    file.open(path);
    if (!file.is_open())
        return;

    init_pvt_key_words (pvt_key_words);

    std::vector<std::any> readed_props(fluid_widget::props_count);

    auto init_default = [&](auto i)
    {
        readed_props[i] = water_props_desc<fluid_widget::all_props[i]>::default_type;
    };
    constexpr_for<0, fluid_widget::props_count>(init_default);

    init_pvt_func (pvt_func);

    int line_num = 1;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        error ret = read_pvt_string(line, line_num, readed_props);
        if (!ret.is_ok())
        {
            printf("%s", ret.description_cstr());
            file.close();
            return;
        }
        line_num++;
    }

    file.close();

    auto write_to_pvt = [&](auto i)
    {
        auto value = readed_props[i];
        using data_type_t = typename water_props_desc<fluid_widget::all_props[i]>::data_type_t;

        widget->set_param<fluid_widget::all_props[i]>(std::any_cast<data_type_t>(value), i);
    };

    constexpr_for<0, fluid_widget::props_count>(write_to_pvt);
}

error set_ref_p (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    res[0] = fmt::string_to_double(value_word);

    return error(OK);
}

error set_ref_fvf (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    res[1] = fmt::string_to_double(value_word);

    return error(OK);
}
error set_compress (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    res[2] = fmt::string_to_double(value_word);

    return error(OK);
}

error set_ref_visc (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    res[3] = fmt::string_to_double(value_word);

    return error(OK);
}

error set_viscoss (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    res[4] = fmt::string_to_double(value_word);

    return error(OK);
}
error set_w_rho (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    res[5] = fmt::string_to_double(value_word);

    return error(OK);
}
error set_w_rho_corr (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    density_correlation type = get_density_correlation_by_string(value_word);
    if (type == density_correlation::COUNT)
        return error(make_error("Wrong word", line));

    res[6] = type;

    return error(OK);
}
error set_w_visc_corr (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    viscosity_correlation type = get_viscosity_correlation_by_string(value_word);
    if (type == viscosity_correlation::COUNT)
        return error(make_error("Wrong word", line));

    res[7] = type;

    return error(OK);
}
error set_w_enth_corr (const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error("Wrong number of fields", line);

    std::string value_word = words[1];

    enthalpy_correlation type = get_enthalpy_correlation_by_string(value_word);
    if (type == enthalpy_correlation::COUNT)
        return error(make_error("Wrong word", line));

    res[8] = type;

    return error(OK);
}
