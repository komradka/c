#include <string>

#pragma once

enum class project_type_t
{
    main,
    network_designer,
    simulator,
    pvt_designer,
    pipe_designer,
    none,

    COUNT
};

std::string get_name_for_gui(project_type_t type);