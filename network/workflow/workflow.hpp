#include <map>

#pragma once

#include "wf_action.hpp"

class workflow
{
private:
    std::map<int, wf_action *> actions;
    int action_count = 0;
    basic_action_storage basic_action;

public:
    workflow();
    ~workflow();

    void create_action(std::string name, std::string gui_name, std::string description, wf_action_category category,
                       types action_return_type, std::vector<types> &args);

    std::map<int, wf_action *> &get_actions() { return actions; }

    wf_action *get_action(int id) {return actions.at(id);}
};