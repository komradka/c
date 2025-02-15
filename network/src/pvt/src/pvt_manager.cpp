#include "pvt_manager.hpp"

pvt_manager::pvt_manager(std::string name)
{
    project_name = name;
}

std::string pvt_manager::get_project_name()
{
    return project_name;
}

fluid_widget *pvt_manager::make_fluid_widget()
{
    widget = new fluid_widget();

    return widget;
}

pvt_manager::~pvt_manager()
{
    delete widget;
}

fluid_widget *pvt_manager::get_widget()
{
    return widget;
}