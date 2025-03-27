#include "reader.hpp"

error res_num_handler(const std::vector<std::string> &words, result_info &res)
{
    sscanf(words[1].c_str(), "%d", &res.res_num);
    return error(OK);
}

error res_name_handler(const std::vector<std::string> &words, result_info &res)
{
    std::string name = "";
    for (int i = 1; i < (int)words.size(); i++)
    {
        name += words[i];
    }
    res.res_name = name;
    return error(OK);
}

error res_gui_dir_handler(const std::vector<std::string> &words, result_info &res)
{
    res.gui_dir = words[1];
    return error(OK);
}

error res_settings_handler(const std::vector<std::string> &words, result_info &res)
{
    res.settings = words[1];
    return error(OK);
}

error res_pvt_dir_handler(const std::vector<std::string> &words, result_info &res)
{
    res.pvt_dir = words[1];
    return error(OK);
}

void reader::init_project_key_words()
{
    project_key_words[kwords.res_num] = res_num_handler;
    project_key_words[kwords.res_name] = res_name_handler;
    project_key_words[kwords.gui_dir_path] = res_gui_dir_handler;
    project_key_words[kwords.settings_path] = res_settings_handler;
    project_key_words[kwords.pvt_dir_path] = res_pvt_dir_handler;
}
