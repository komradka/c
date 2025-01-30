#include "reader.hpp"

error set_use_temperature(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_use_hb(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_vis_ia(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_solver_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_linear_solver_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_ai_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_precond_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_rate_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_max_linear_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_max_newton_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_linear_eps_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_newton_eps_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);
error set_root_diff_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res);

error reader::read_settings(std::string filename, settings_dialog *settings)
{
    if (file.is_open())
        file.close();

    file.open(filename);
    if (!file.is_open())
    {
        return error("Unable to open file", filename);
    }
    file_name = filename;

    init_settings_key_words();

    std::vector<std::any> readed_settings(settings_dialog::setting_count);

    auto init_default = [&](auto i)
    {
        readed_settings[i] = network_solver_setting_desc<settings_dialog::all_settings[i]>::default_type;
    };

    constexpr_for<0, settings_dialog::setting_count>(init_default);

    init_settings_func();

    int line_num = 1;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        RETURN_IF_FAIL(read_settings_string(line, line_num, readed_settings));
        line_num++;
    }

    file.close();

    auto write_to_settings = [&](auto i)
    {
        auto value = readed_settings[i];
        using data_type_t = typename network_solver_setting_desc<settings_dialog::all_settings[i]>::data_type_t;

        settings->set_param<settings_dialog::all_settings[i]>(std::any_cast<data_type_t>(value), i);
    };

    constexpr_for<0, settings_dialog::setting_count>(write_to_settings);

    return error(OK);
}

void reader::init_settings_key_words()
{
    auto init = [&](auto i)
    {
        settings_key_words[network_solver_setting_desc<settings_dialog::all_settings[i]>::name] = i;
    };

    constexpr_for<0, settings_dialog::setting_count>(init);
}

void reader::init_settings_func()
{
    settings_func[0] = set_use_temperature;
    settings_func[1] = set_use_hb;
    settings_func[2] = set_vis_ia;
    settings_func[3] = set_solver_type;
    settings_func[4] = set_linear_solver_type;
    settings_func[5] = set_ai_type;
    settings_func[6] = set_precond_type;
    settings_func[7] = set_rate_type;
    settings_func[8] = set_max_linear_iter;
    settings_func[9] = set_max_newton_iter;
    settings_func[10] = set_linear_eps_iter;
    settings_func[11] = set_newton_eps_iter;
    settings_func[12] = set_root_diff_iter;

    if (settings_func.size() != settings_dialog::setting_count)
        abort();
}

error reader::read_settings_string(const std::string str, const int line, std::vector<std::any> &readed_settings)
{
    std::vector<std::string> words;
    split_string(str, words);

    if (words.size() < 1)
    {
        return error(make_error("Wrong number of fields", line), file_name);
    }
    std::string main_word = words[0];

    if (settings_key_words.count(main_word))
    {
        int index = settings_key_words.at(main_word);
        RETURN_IF_FAIL(settings_func.at(index)(words, line, readed_settings));
        return error(OK);
    }

    return error(make_error("Unknown word: " + main_word, line), file_name);
}

// settings read fuction

error set_use_temperature(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));

    std::string value_word = words[1];

    if (value_word == "True")
    {
        res[0] = true;
        return error(OK);
    }
    else if (value_word == "False")
    {
        res[0] = false;
        return error(OK);
    }

    return error(make_error("Wrong word", line));
}

error set_use_hb(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));

    std::string value_word = words[1];

    if (value_word == "True")
    {
        res[1] = true;
        return error(OK);
    }
    else if (value_word == "False")
    {
        res[1] = false;
        return error(OK);
    }

    return error(make_error("Wrong word", line));
}

error set_vis_ia(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));

    std::string value_word = words[1];

    if (value_word == "True")
    {
        res[2] = true;
        return error(OK);
    }
    else if (value_word == "False")
    {
        res[2] = false;
        return error(OK);
    }

    return error(make_error("Wrong word", line));
}

error set_solver_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    solver_type type = get_solver_type_by_string(value_word);
    if (type == solver_type::COUNT)
        return error(make_error("Wrong word", line));
    res[3] = type;

    return error(OK);
}

error set_linear_solver_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    linear_solver_type type = get_liner_solver_type_by_string(value_word);
    if (type == linear_solver_type::COUNT)
        return error(make_error("Wrong word", line));
    res[4] = type;

    return error(OK);
}

error set_ai_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    initial_approximation_type type = get_initial_approximation_type_by_string(value_word);
    if (type == initial_approximation_type::COUNT)
        return error(make_error("Wrong word", line));
    res[5] = type;

    return error(OK);
}

error set_precond_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    preconditioner_type type = get_preconditioner_type_by_string(value_word);
    if (type == preconditioner_type::COUNT)
        return error(make_error("Wrong word", line));
    res[6] = type;

    return error(OK);
}

error set_rate_type(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    rate_type type = get_rate_type_by_string(value_word);
    if (type == rate_type::COUNT)
        return error(make_error("Wrong word", line));
    res[7] = type;

    return error(OK);
}

error set_max_linear_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    int iter;
    if (sscanf(value_word.c_str(), "%d", &iter) != 1)
        return error(make_error("Wrong word", line));
    res[8] = iter;
    return error(OK);
}

error set_max_newton_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    int iter;
    if (sscanf(value_word.c_str(), "%d", &iter) != 1)
        return error(make_error("Wrong word", line));
    res[9] = iter;
    return error(OK);
}

error set_linear_eps_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    double eps;
    if (sscanf(value_word.c_str(), "%lf", &eps) != 1)
        return error(make_error("Wrong word", line));
    res[10] = eps;
    return error(OK);
}

error set_newton_eps_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    double eps;
    if (sscanf(value_word.c_str(), "%lf", &eps) != 1)
        return error(make_error("Wrong word", line));
    res[10] = eps;
    return error(OK);
}

error set_root_diff_iter(const std::vector<std::string> &words, const int line, std::vector<std::any> &res)
{
    if (words.size() != 2)
        return error(make_error("Wrong number of fields", line));
    std::string value_word = words[1];

    double eps;
    if (sscanf(value_word.c_str(), "%lf", &eps) != 1)
        return error(make_error("Wrong word", line));
    res[11] = eps;
    return error(OK);
}
