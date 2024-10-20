#include <string>

#ifndef OT

#define OT

enum class network_objects
{
    sink,
    source,

    unknown
};

std::string get_name_for_gui(network_objects &obj);

network_objects string_to_type (const std::string &str);

#endif