#include "work_area_tree.hpp"
#include "wf_action_treeitem.hpp"

void work_area::add_action(QTreeWidgetItem *item, int)
{
    wf_action_treeitem *action_item = dynamic_cast<wf_action_treeitem *>(item);

    if (action_item->is_root_item())
        return;

    add_item(action_item->get_action());
}

void work_area::add_item(wf_action *act)
{
    wf_action_areaitem *item = new wf_action_areaitem(act->gui_name, act->description, act->id, this);
    actions.push_back(item);
}

work_area::work_area(QWidget *parent) : QTreeWidget(parent)
{
    setHeaderLabel("Work Area");
    setColumnCount(1);
    setStyleSheet("QTreeWidget { font-size: 14pt;}"
                  "QTreeWidget::item { height: 32px; border: 1px solid black; color : black;}");
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    showDropIndicator();
    setDragDropMode(QTreeWidget::InternalMove);
}