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
    wf_action_areaitem *item = new wf_action_areaitem(act, this);
    actions.push_back(item);

    switch (act->utility)
    {
    case wf_action_utility::none:
    {
        item->setText(1, "None");
        break;
    }
    case wf_action_utility::string:
    {
        QPushButton *line = new QPushButton("Sdad");
        setItemWidget(item, 1, line);
        break;
    }
    case wf_action_utility::file:
    {
        QLineEdit *line = new QLineEdit();
        setItemWidget(item, 1, line);
        break;
    }
    case wf_action_utility::COUNT:
    {
        break;
    }
    }
}

work_area::work_area(QWidget *parent) : QTreeWidget(parent)
{
    setColumnCount(2);
    setHeaderLabels({"Work Area", "Utility"});
    header()->resizeSection(0, parent->width() * 0.5);

    setStyleSheet("QTreeWidget { font-size: 14pt;}"
                  "QTreeWidget::item { height: 32px; border: 1px solid black; color : black;}");
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    showDropIndicator();
    setDragDropMode(QTreeWidget::DragDrop);
}

Qt::DropActions work_area::supportedDropActions() const
{
    return Qt::MoveAction;
}