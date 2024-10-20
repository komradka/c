#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "../error.hpp"
#include "../kernel/topology.hpp"

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
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, const string &filename, graph *)>> topology_key_words;

public:
    ~reader()
    {
        if (file.is_open())
        {
            file.close();
        }
    }

    error read_topology(std::string filename, graph *topology)
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
            RETURN_IF_FAIL(read_topology_string(line, line_num, topology));
            line_num++;
        }

        file.close();
        return error("Wrong number of fields", 0);
    }

private:
    error read_topology_string(const std::string str, const int line, graph *topology);
    void init_topology_key_words();
};

#endif