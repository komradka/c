#include "workflow.hpp"

#include <iostream>
workflow::workflow()
{
    for (int i = 0; i < basic_action_storage::basic_action_count; i++)
    {
        auto act = basic_action.basic_action[i];
        create_action(act->name, act->gui_name, act->description, act->category);
    }
}

workflow::~workflow()
{
    action_count = 0;
    for (auto act : actions)
    {
        delete act.second;
    }
}

void workflow::create_action(std::string name, std::string gui_name, std::string description, wf_action_category category)
{
    action_count++;
    wf_action *act = new wf_action(name, gui_name, description, category, action_count);
    actions[action_count] = act;
}