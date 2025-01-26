#include <string>

#ifndef OT

#define OT

enum class network_objects
{
    sink,
    source,
    pipe,
    joint,

    unknown,
    link,

    COUNT
};

std::string get_name(network_objects obj);

std::string get_name_for_gui(network_objects obj);

network_objects string_to_type (const std::string &str);

#endif