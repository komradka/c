#include <iostream>
#include <optional>
#include <string>
#include <vector>

#pragma once

class object_data
{
public:
    std::string name;
    std::optional<bool> activity;

    std::string get_name_for_gui()
    {
        return name;
    }

    bool is_active() { return activity.value(); }
};
