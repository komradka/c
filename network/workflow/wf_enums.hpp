#include <string>

#pragma once

enum class wf_action_category
{
    maker,
    getters,
    setters,
    objects_type,
    param_type,
    utility,
    custom,

    COUNT
};

std::string get_name_for_gui(wf_action_category e);