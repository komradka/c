#include "object_tree_item.hpp"
#include "object_tree.hpp"

object_tree_item::object_tree_item(network_objects type, objects_tree *parent) : QTreeWidgetItem(parent)
{
    setText(0, QString::fromStdString(get_name_for_gui(type) + "s"));
    parent->roots[type] = this;
    parent->addTopLevelItem(this);
}

object_tree_item::object_tree_item(std::string name, network_objects type, objects_tree *parent) : QTreeWidgetItem()
{
    setText(0, QString::fromStdString(name));
    parent->roots[type]->addChild(this);
}
