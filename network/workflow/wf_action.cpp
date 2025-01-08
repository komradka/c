#include "wf_action.hpp"

wf_action::wf_action(std::string name, std::string gui, std::string description, wf_action_category category, int id,
                     types action_return_type, std::vector<types> &args, wf_action_utility utility)
{
    this->name = name;
    this->gui_name = gui;
    this->description = description;
    this->category = category;
    this->id = id;

    this->action_return_type = action_return_type;
    this->args = args;
    this->utility = utility;
}