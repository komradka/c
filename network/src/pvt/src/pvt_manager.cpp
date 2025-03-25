#include "pvt_manager.hpp"

pvt_manager::pvt_manager(std::string name)
{
    std::replace(name.begin(), name.end(), ' ', '_');
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

error pvt_manager::write_fluid(std::ofstream &data_writer)
{
    if (!data_writer.is_open())
    {
        return error ("Cannot write fluid");
    }

    auto write = [&](auto i)
    {
        if (widget == nullptr)
            return;
        data_writer << water_props_desc<widget->all_props[i]>::name << " ";
        data_writer << get_name(widget->get_param<widget->all_props[i]>()) << std::endl;
    };

    constexpr_for<0, widget->props_count>(write);

    return error(OK);
}
