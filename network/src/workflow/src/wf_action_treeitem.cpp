#include "wf_action_treeitem.hpp"
#include "wf_action_tree.hpp"

wf_action_treeitem::wf_action_treeitem(wf_action_category category, action_tree *parent) : QTreeWidgetItem(parent)
{
    this->category = category;
    this->is_root = true;

    tree_name = get_name_for_gui(category);
    parent->roots[category] = this;
    parent->addTopLevelItem(this);

    setText(0, QString::fromStdString(tree_name));
}

wf_action_treeitem::wf_action_treeitem(wf_action *action, action_tree *parent)
{
    this->category = action->category;
    this->is_root = false;
    this->id = action->id;
    this->action = action;

    tree_name = action->gui_name;
    parent->roots[category]->addChild(this);

    setToolTip(0, QString::fromStdString(action->description));
    setText(0, QString::fromStdString(tree_name));
}