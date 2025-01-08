#include "workflow.hpp"

#include <iostream>
workflow::workflow()
{
    for (unsigned int i = 0; i < basic_action.basic_action.size(); i++)
    {
        auto act = basic_action.basic_action[i];
        create_action(act->name, act->gui_name, act->description, act->category, act->action_return_type, act->args, act->utility);
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

void workflow::create_action(std::string name, std::string gui_name, std::string description, wf_action_category category,
                             types action_return_type, std::vector<types> &args, wf_action_utility utility)
{
    action_count++;
    wf_action *act = new wf_action(name, gui_name, description, category, action_count, action_return_type, args, utility);
    actions[action_count] = act;
}