#include <string>
#include <vector>
#include "../wf_enums.hpp"
#pragma once

struct abstract_basic_action
{
    std::string name;
    std::string gui_name;
    std::string description;
    types action_return_type;
    std::vector<types> args;

    wf_action_category category;
    wf_action_utility utility;
};

std::string make_description(std::string desc, std::vector<types> args, types return_type);

struct make_object_action : public abstract_basic_action
{
    make_object_action();
};

struct make_link_action : public abstract_basic_action
{
    make_link_action();
};

struct get_object_action : public abstract_basic_action
{
    get_object_action();
};

