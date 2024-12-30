#include "wf_action.hpp"

wf_action::wf_action(std::string name, std::string gui, std::string description, wf_action_category category, int id)
{
    this->name = name;
    this->gui_name = gui;
    this->description = description;
    this->category = category;
    this->id = id;
}