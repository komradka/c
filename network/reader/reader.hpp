#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "../error.hpp"
#include "../kernel/topology.hpp"
#include "../manager_gui/result_info_storage.hpp"
#include "../gui/graph_area.hpp"

#ifndef READER

#define READER

enum class read_mode
{
    topology,
    fluid,
    main
};

void split_string(const std::string &str, std::vector<std::string> &words);

std::string make_error(const std::string &err_message, const int line);

class reader
{
private:
    std::ifstream file;
    std::string file_name;

    std::map<std::string, std::function<error(const std::vector<std::string> &, const string &filename, const int)>> main_key_words;
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, const string &filename, graph_area *)>> topology_key_words;
    std::map<std::string, std::function<error(const std::vector<std::string> &, result_info &)>> project_key_words;
    std::map<std::string, int> settings_key_words;
    std::map<int, std::function<error(const std::vector<std::string> &, const int, std::vector<std::any> &)>> settings_func;

public:
    ~reader()
    {
        if (file.is_open())
        {
            file.close();
        }
    }

    error read_data(result_info &res, graph_area *gui_manager, settings_dialog *settings)
    {
        RETURN_IF_FAIL(read_topology(res.gui_dir + "/GUI.data", gui_manager));
        RETURN_IF_FAIL(read_settings(res.settings, settings));
        return error(OK);
    }

private:
    error read_settings(std::string filename, settings_dialog *settings);

    error read_topology(std::string filename, graph_area *gui_manager)
    {
        if (file.is_open())
            file.close();

        file.open(filename);
        if (!file.is_open())
        {
            return error("Unable to open file", filename);
        }
        file_name = filename;

        init_topology_key_words();

        int line_num = 1;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }
            RETURN_IF_FAIL(read_topology_string(line, line_num, gui_manager));
            line_num++;
        }

        file.close();
        return error("Wrong number of fields", 0);
    }

public:
    error read_project(std::string dirname, std::vector<result_info> &results)
    {
        if (file.is_open())
            file.close();

        file_name = dirname + "/project.np";

        file.open(file_name);
        if (!file.is_open())
        {
            return error("Unable to open project file");
        }

        init_project_key_words();

        std::string line;
        result_info buf;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                results.push_back(buf);
                continue;
            }
            std::vector<std::string> words;
            split_string(line, words);

            project_key_words[words[0]](words, buf);
        }
        results.push_back(buf);

        return error(OK);
    }

private:
    error read_topology_string(const std::string str, const int line, graph_area *gui_manager);
    error read_settings_string(const std::string str, const int line, std::vector<std::any> &readed_settings);
    void init_topology_key_words();
    void init_project_key_words();
    void init_settings_key_words();
    void init_settings_func();
};

#endif