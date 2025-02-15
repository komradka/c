#include <string>
#include "fluid_widget.hpp"

class pvt_manager
{
private:
    std::string project_name;
    fluid_widget *widget;

public:
    pvt_manager(std::string name);
    ~pvt_manager();

    std::string get_project_name();

    fluid_widget *make_fluid_widget();

    fluid_widget *get_widget();
};