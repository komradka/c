#include "wf_enums.hpp"

std::string get_name_for_gui(wf_action_category e)
{
    switch (e)
    {
    case wf_action_category::custom:
        return "Custom Action";
    case wf_action_category::getters:
        return "Getters";
    case wf_action_category::maker:
        return "Create";
    case wf_action_category::objects_type:
        return "Network Objects Type";
    case wf_action_category::param_type:
        return "Network Parameters";
    case wf_action_category::setters:
        return "Setters";
    case wf_action_category::utility:
        return "Utility";
    case wf_action_category::COUNT:
        return "";
    }
    return "";
}

std::string get_name_for_gui(types e)
{
    switch (e)
    {
    case types::Void:
        return "Void";
    case types::String:
        return "String";
    case types::Object_type:
        return "Object Type";
    case types::Object:
        return "Network Object";
    case types::Int:
        return "Int";
    case types::Float:
        return "Float";
    case types::File:
        return "File";
    case types::COUNT:
        return "";
    }
    return "";
}