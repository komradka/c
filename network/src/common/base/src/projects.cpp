
#include "projects.hpp"

std::string get_name_for_gui(project_type_t type)
{
    switch (type)
    {
    case project_type_t::main:
        return "Main";
    case project_type_t::network_designer:
        return "Network Designer";
    case project_type_t::pipe_designer:
        return "Pipe Designer";
    case project_type_t::pvt_designer:
        return "PVT Designer";
    case project_type_t::simulator:
        return "hydrodynamics Designer";
    case project_type_t::none:
        return "No Project";
    case project_type_t::COUNT:
        return "";
    }
    return "";
}