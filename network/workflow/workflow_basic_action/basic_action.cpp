#include "basic_action.hpp"

std::string make_description(std::string desc, std::vector<types> args, types return_type)
{
    std::string res = desc + "\n";
    res += "Args:\n";
    for (unsigned int i = 0; i < args.size(); i++)
    {
        res += "  " + get_name_for_gui(args[i]) + "\n";
    }
    res += "Return Type:\n";
    res += "  " + get_name_for_gui(return_type);
    return res;
}

make_object_action::make_object_action()
{
    name = "MO";
    gui_name = "Make Object";
    description = make_description("Create network object", {types::Object_type, types::File}, types::Object);
    // description = "Create network object\nArgs:\nNetwork Objects Type: Type\nFile: Object Data File\nReturn Type:\nObject.";
    category = wf_action_category::maker;
    action_return_type = types::Object;
    args.push_back(types::Object_type);
    args.push_back(types::File);
    utility = wf_action_utility::none;
}

make_link_action::make_link_action()
{
    name = "ML";
    gui_name = "Make Link";
    description = make_description("Create link between two objects", {types::Object, types::Object}, types::Void);
    category = wf_action_category::maker;
    action_return_type = types::Void;
    args.push_back(types::Object);
    args.push_back(types::Object);
    utility = wf_action_utility::none;
}

get_object_action::get_object_action()
{
    name = "GETO";
    gui_name = "Get Object";
    description = make_description("Get object by name", {types::String}, types::Object);
    category = wf_action_category::getters;
    action_return_type = types::Object;
    args.push_back(types::String);
    utility = wf_action_utility::string;
}
