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
    ~pvt_manager();

    std::string get_project_name();

    fluid_widget *make_fluid_widget();

    fluid_widget *get_widget();

    error write_fluid(std::ofstream &data_writer);
};
