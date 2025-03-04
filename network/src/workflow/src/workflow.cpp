#include "workflow.hpp"
#include "workflow_dialog.hpp"

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

std::string workflow::get_info(workflow_dialog *dialog)
{
    std::string info;
    info += "Workflow:\n";
    work_area *area = dialog->get_work_area();
    info += "Action Count: " + std::to_string(area->topLevelItemCount()) + "\n";

    return info;
}

error workflow::calculate(report_system *rep, workflow_dialog *dialog)
{
    rep->print(message_type::MESSAGE, message_source::WF, message_category::ALL, "Starting workflow calculation");
    rep->print(message_type::MESSAGE, message_source::WF, message_category::ALL, "%s", get_info(dialog));
    work_area *area = dialog->get_work_area();

    wf_action_areaitem *area_item;

    int root_item_count = area->topLevelItemCount();

    for (int i = 0; i < root_item_count; i++)
    {
        area_item = dynamic_cast<wf_action_areaitem *>(area->topLevelItem(i));
        error err = action_holder(area_item);
        if (!err.is_ok())
        {
            rep->print(message_type::ERR, message_source::WF, message_category::ALL, "Cannot perform action %d: %s",
                       i + 1, actions.at(area_item->get_id())->gui_name.c_str());
            rep->print(message_type::ERR, message_source::WF, message_category::ALL, "%s", err.description_cstr());
            break;
        }
    }

    return error(OK);
}
