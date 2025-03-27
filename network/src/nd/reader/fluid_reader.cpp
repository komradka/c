#include "reader.hpp"

error make_fluid (const std::vector<std::string> &words, const std::string pvt_path, const int line, fluid_dialog *fluids);
error choose_fluid (const std::vector<std::string> &words, const std::string pvt_path, const int line, fluid_dialog *fluids);

error reader::read_pvt(result_info &res, fluid_dialog *fluids)
{
    std::string fluid_data_file = project_dir + "/" + res.pvt_dir + "/" + paths.pvt_data;

    if (file.is_open())
        file.close();

    file.open(fluid_data_file);
    if (!file.is_open())
    {
        return error("Unable to open file", fluid_data_file);
    }
    file_name = fluid_data_file;

    init_pvt_key_words();

    int line_num = 1;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        RETURN_IF_FAIL(read_pvt_string(line, line_num, fluids));
        line_num++;
    }

    file.close();
    return error("Wrong number of fields", 0);
}

void reader::init_pvt_key_words()
{
    pvt_key_words[kwords.make_fluid] = make_fluid;
    pvt_key_words[kwords.curr_fluid] = choose_fluid;
}

error reader::read_pvt_string(const std::string str, const int line, fluid_dialog *fluids)
{
    std::string pvt_path = project_dir;

    std::vector<std::string> words;
    fmt::split_string(str, words);

    if (words.size() < 1)
    {
        return error(make_error("Wrong number of fields", line), file_name);
    }

    std::string main_word = words[0];

    if (pvt_key_words.find(main_word) != pvt_key_words.end())
        return pvt_key_words[main_word](words, pvt_path, line, fluids);

    return error(make_error("Unknown word: " + main_word, line), file_name);
}

error make_fluid (const std::vector<std::string> &words, const std::string pvt_path, const int line, fluid_dialog *fluids)
{
    if (words.size() != 3)
        return error(make_error("Wrong number of fields", line), "");

    std::string name = words[1];
    std::string path = pvt_path + "/" + words[2];

    fluids->export_fluid(name, path);

    return error(OK);
}

error choose_fluid (const std::vector<std::string> &words, const std::string pvt_path, const int line, fluid_dialog *fluids)
{
    return error(OK);
}
