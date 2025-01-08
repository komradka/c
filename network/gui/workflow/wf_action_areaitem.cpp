#include "wf_action_areaitem.hpp"

wf_action_areaitem::wf_action_areaitem(wf_action *act, QTreeWidget *parent) : QTreeWidgetItem(parent)
{
    this->name = act->gui_name;
    this->id = act->id;
    this->utility = act->utility;
    parent->addTopLevelItem(this);
    setText(0, QString::fromStdString(act->gui_name));
    setToolTip(0, QString::fromStdString(act->description));
}