#include "objects_type.hpp"

std::string get_name_for_gui(network_objects obj)
{
    switch (obj)
    {
        case network_objects::sink: return "Sink"; break;
        case network_objects::source: return "Source"; break;
        case network_objects::link: return "Link"; break;
    }
    return "Unknown object";
}

network_objects string_to_type(const std::string &str)
{
    if (str == "SOURCE")
        return network_objects::source;
    if (str == "SINK")
        return network_objects::sink;
    if (str == "LINK")
        return network_objects::link;
        
    return network_objects::unknown;
}