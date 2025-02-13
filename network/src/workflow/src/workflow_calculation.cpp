#include "workflow.hpp"
#include "workflow_dialog.hpp"

std::vector<wf_action_areaitem *> get_args(wf_action_areaitem *area_item)
{
    std::vector<wf_action_areaitem *> res;
    int given_args_count = area_item->childCount();
    for (int i = 0; i < given_args_count; i++)
    {
        wf_action_areaitem *areaitem = dynamic_cast<wf_action_areaitem *>(area_item->child(i));
        res.push_back(areaitem);
    }

    return res;
}

error workflow::check_args(wf_action_areaitem *area_item)
{
    std::vector<types> required_args = actions.at(area_item->get_id())->args;
    std::vector<wf_action_areaitem *> given_args = get_args(area_item);

    if (given_args.size() != required_args.size())
    {
        return error("Expects " + std::to_string(required_args.size()) + " argument, " + std::to_string(given_args.size()) + " provided.");
    }

    for (unsigned int i = 0; i < required_args.size(); i++)
    {
        types type = required_args[i];
        if (type != actions.at(area_item->get_id())->action_return_type)
        {
            return error("Expected that argument " + std::to_string(i + 1) + " be a " + get_name_for_gui(type) + ", " +
                         get_name_for_gui(actions.at(area_item->get_id())->action_return_type) + " provided.");
        }
    }

    return error(OK);
}

error workflow::action_holder(wf_action_areaitem *area_item)
{
    // std::vector<types> required_args = actions.at(area_item->get_id())->args;
    RETURN_IF_FAIL(check_args(area_item));

    return error(OK);
}