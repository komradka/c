#include <string>
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
};

struct make_object_action : public abstract_basic_action
{
    make_object_action()
    {
        name = "MO";
        gui_name = "Make Object";
        description = "Create network object\nArgs:\nNetwork Objects Type: Type\nFile: Object Data File\nReturn Type:\nObject.";
        category = wf_action_category::maker;
        action_return_type = types::Object;
        args.push_back(types::Object_type);
        args.push_back(types::File);
    }
};