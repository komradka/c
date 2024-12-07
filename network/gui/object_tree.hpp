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
        connect(this, SIGNAL(delete_object(int)), parent, SLOT(delete_object(int)));
        connect(this, SIGNAL(delete_link(int)), parent, SLOT(delete_link(int)));
        connect(this, SIGNAL(itemSelectionChanged()), parent, SLOT(change_selection_items()));

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

    void add_object(network_objects type, std::string &name, int id, QGraphicsItem *graph_item)
    {
        object_tree_item *item = new object_tree_item(name, type, graph_item, id, this);
        Q_UNUSED(item);
    }

    void delete_link_item(int id)
    {
        QList<QTreeWidgetItem *> link_items = roots.at(network_objects::link)->takeChildren();

        for (auto item : link_items)
        {
            object_tree_item *si = dynamic_cast<object_tree_item *>(item);

            if (si->get_id() == id)
            {
                delete item;
                return;
            }
        }
    }

    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Delete)
        {
            object_tree_item *si = dynamic_cast<object_tree_item *>(selectedItems()[0]);
            int id = si->get_id();

            delete selectedItems()[0];

            if (si->is_link())
                emit delete_link(id);
            else
                emit delete_object(id);
        }
    }

signals:
    void delete_object(int id);
    void delete_link(int id);

public:
    friend class object_tree_item;
};