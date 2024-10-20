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
        setColumnCount(1);

        setHeaderLabel("Objects");

        for (int i = 0; i < (int)network_objects::COUNT - 2; i++)
        {
            const auto type = static_cast<network_objects>(i);

            object_tree_item *obj = new object_tree_item(type, this);
        }

        object_tree_item *link_item = new object_tree_item(network_objects::link, this);
    }

    void add_object(network_objects type, std::string &name)
    {
        object_tree_item *item = new object_tree_item(name, type, this);
    }

    friend class object_tree_item;
};