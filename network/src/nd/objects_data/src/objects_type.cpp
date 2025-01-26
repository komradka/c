#include "objects_type.hpp"

std::string get_name_for_gui(network_objects obj)
{
    switch (obj)
    {
    case network_objects::sink:
        return "Sink";
        break;
    case network_objects::source:
        return "Source";
        break;
    case network_objects::link:
        return "Link";
        break;
    case network_objects::pipe:
        return "Pipe";
        break;
    case network_objects::joint:
        return "Joint";
        break;
    case network_objects::unknown:
    case network_objects::COUNT:
        return "error";
        break;
    }

    return "Unknown object";
}

std::string get_name(network_objects obj)
{
    switch (obj)
    {
    case network_objects::sink:
        return "SINK";
        break;
    case network_objects::source:
        return "SOURCE";
        break;
    case network_objects::link:
        return "LINK";
        break;
    case network_objects::pipe:
        return "PIPE";
        break;
    case network_objects::joint:
        return "JOINT";
        break;
    case network_objects::unknown:
    case network_objects::COUNT:
        return "ERROR";
    }
    return "UNKNOWN";
}

network_objects string_to_type(const std::string &str)
{
    if (str == "SOURCE")
        return network_objects::source;
    if (str == "SINK")
        return network_objects::sink;
    if (str == "LINK")
        return network_objects::link;
    if (str == "PIPE")
        return network_objects::pipe;
    if (str == "JOINT")
        return network_objects::joint;

    return network_objects::unknown;
}