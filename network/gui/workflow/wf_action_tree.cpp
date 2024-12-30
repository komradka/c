#include "wf_action_tree.hpp"

void action_tree::fill_action(workflow *wf)
{
    fill_root_elements();
    fill_sub_element(wf);
}

void action_tree::fill_root_elements()
{
    for (int i = 0; i < (int)wf_action_category::COUNT; i++)
    {
        wf_action_treeitem *item = new wf_action_treeitem((wf_action_category)i, this);
        Q_UNUSED(item);
    }
}

void action_tree::fill_sub_element(workflow *wf)
{
    std::map<int, wf_action *> &actions = wf->get_actions();

    for (auto act_it : actions)
    {
        wf_action *act = act_it.second;
        wf_action_treeitem *item = new wf_action_treeitem(act, this);
    }
}