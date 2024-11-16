#include "object_tree_item.hpp"
#pragma once

class objects_tree : public QTreeWidget
{
    Q_OBJECT

private:
    std::map<network_objects, QTreeWidgetItem *> roots;

public:
    objects_tree(QWidget *parent) : QTreeWidget(parent)
    {
        setSelectionMode(QAbstractItemView::MultiSelection);

        setColumnCount(1);

        setHeaderLabel("Objects");

        for (int i = 0; i < (int)network_objects::COUNT - 2; i++)
        {
            const auto type = static_cast<network_objects>(i);

            object_tree_item *obj = new object_tree_item(type, this);
            Q_UNUSED(obj);
        }

        object_tree_item *link_item = new object_tree_item(network_objects::link, this);
        Q_UNUSED(link_item);
    }

    void add_object(network_objects type, std::string &name, QGraphicsItem *graph_item)
    {
        object_tree_item *item = new object_tree_item(name, type, graph_item, this);
        Q_UNUSED(item);
    }

    void keyPressEvent(QKeyEvent *event)
    {
        Q_UNUSED(event);
        // if (event->key() == Qt::Key_Delete)
        // {
        //     object_tree_item *si = dynamic_cast<object_tree_item *>(selectedItems()[0]);
        // }
    }

    friend class object_tree_item;
};