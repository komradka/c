#include "workflow.hpp"
#include "workflow_dialog.hpp"

error workflow::check_args(wf_action_areaitem *area_item)
{

}

error workflow::action_holder(wf_action_areaitem *area_item)
{
    // std::vector<types> required_args = actions.at(area_item->get_id())->args;
    RETURN_IF_FAIL(check_args(area_item));
}