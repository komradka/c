#include <string>
#include <fstream>

#include "error.hpp"
#include "fluid_widget.hpp"

#pragma once

class pvt_manager
{
private:
    std::string project_name;
    fluid_widget *widget = nullptr;

public:
    pvt_manager(std::string name);
    pvt_manager(std::string name, std::string path);
    ~pvt_manager();

    std::string get_project_name();

    fluid_widget *make_fluid_widget();

    fluid_widget *get_widget();

    error write_fluid(std::ofstream &data_writer);

private:
    std::map<std::string, int> pvt_key_words;
    std::map<int, std::function<error(const std::vector<std::string> &, const int, std::vector<std::any> &)>> pvt_func;

    void read_pvt(std::string path);
    error read_pvt_string(const std::string str, const int line, std::vector<std::any> &readed_settings);
};
