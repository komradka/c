#include "reader.hpp"

void split_string(const std::string &str, std::vector<std::string> &words)
{
    std::stringstream ss(str);
    std::string word;
    while (ss >> word)
    {
        words.push_back(word);
    }
}

std::string make_error(const std::string &err_message, const int line)
{
    return "Wrong file format: " + err_message + ".\n" + "In line - " + std::to_string(line);
}

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
        name += words[i] + " ";
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

void reader::init_project_key_words()
{
    project_key_words["RES_NUM"] = res_num_handler;
    project_key_words["RES_NAME"] = res_name_handler;
    project_key_words["GUI_DIR"] = res_gui_dir_handler;
    project_key_words["SETTINGS"] = res_settings_handler;
}