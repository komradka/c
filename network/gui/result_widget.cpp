#include "result_widget.hpp"

void result_widget::reprint_table()
{
    std::vector<int> checked_q = get_checked_items();
    std::vector<QTreeWidgetItem *> checked_objs = get_checked_objects();

    int objs_count = checked_objs.size();
    int q_count = checked_q.size();

    table->setColumnCount(q_count);
    table->setRowCount(objs_count);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList horizontal_headers;
    QStringList vertical_headers;
    for (int i : checked_q)
    {
        horizontal_headers << QString::fromStdString(get_name_for_gui(phys_q_type(i)));
    }

    for (QTreeWidgetItem *it : checked_objs)
    {
        object_tree_item *si = dynamic_cast<object_tree_item *>(it);

        vertical_headers << QString::fromStdString(si->get_name());
    }

    table->setHorizontalHeaderLabels(horizontal_headers);
    table->setVerticalHeaderLabels(vertical_headers);

    for (int i = 0; i < objs_count; i++)
    {
        object_tree_item *si = dynamic_cast<object_tree_item *>(checked_objs[i]);
        vertex *v = si->get_object();

        if (object_results.count(v->get_id()))
        {
            for (int j = 0; j < q_count; j++)
            {
                phys_q_type q = phys_q_type(checked_q[j]);

                double value = object_results.at(v->get_id())[q];

                table->setItem(i, j, new QTableWidgetItem(QString::number(value)));
            }
        }
    }
}

std::vector<int> result_widget::get_checked_items()
{
    std::vector<int> res;
    for (int i = 0; i < quantity_list->count(); i++)
    {
        QListWidgetItem *item = quantity_list->item(i);
        if (item->checkState() == Qt::Checked)
        {
            res.push_back(i);
        }
    }
    return res;
}

std::vector<QTreeWidgetItem *> result_widget::get_checked_objects()
{
    QList<QTreeWidgetItem *> checked_objs = objects_list->selectedItems();

    std::vector<QTreeWidgetItem *> res;

    for (QTreeWidgetItem *it : checked_objs)
    {
        object_tree_item *si = dynamic_cast<object_tree_item *>(it);
        if (!si->is_link())
            res.push_back(it);
    }

    return res;
}