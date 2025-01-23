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

    action_count++;
    item->inner_id = action_count;

    actions.push_back(item);

    switch (act->utility)
    {
    case wf_action_utility::none:
    {
        item->setText(1, "None");
        item->utility_widget = nullptr;
        break;
    }
    case wf_action_utility::string:
    {
        QLineEdit *line = new QLineEdit();
        setItemWidget(item, 1, line);
        item->utility_widget = line;
        break;
    }
    case wf_action_utility::file:
    {
        QLineEdit *line = new QLineEdit();
        setItemWidget(item, 1, line);
        item->utility_widget = line;
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
    setDragDropMode(QTreeWidget::InternalMove);
}

Qt::DropActions work_area::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

void work_area::dropEvent(QDropEvent *event)
{
    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        wf_action_areaitem *area_item = dynamic_cast<wf_action_areaitem *>(*it);
        area_item->save_utility();
        it++;
    }

    QTreeWidget::dropEvent(event);

    QTreeWidgetItemIterator new_it(this);
    while (*new_it)
    {
        wf_action_areaitem *area_item = dynamic_cast<wf_action_areaitem *>(*new_it);
        QWidget *utility = area_item->restore_utility();

        if (utility != nullptr)
            setItemWidget(area_item, 1, utility);
        else
            area_item->setText(1, "None");
        new_it++;
    }
}

void work_area::update_widgets()
{
}
