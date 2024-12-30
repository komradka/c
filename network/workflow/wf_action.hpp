#include <string>
#include <vector>
#include "workflow_basic_action/basic_action_storage.hpp"

#pragma once

class wf_action
{
public:
    std::string name;
    std::string gui_name;
    std::string description;

    wf_action_category category;

    int id;

    types action_return_type;
    std::vector<types> args;

public:
    wf_action(std::string name, std::string gui, std::string description, wf_action_category category, int id,
              types action_return_type, std::vector<types> &args);
};
