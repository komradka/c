#include "object_tree_item.hpp"
#include "object_tree.hpp"

object_tree_item::object_tree_item(network_objects type, objects_tree *parent) : QTreeWidgetItem(parent)
{
    setText(0, QString::fromStdString(get_name_for_gui(type) + "s"));
    auto currentFlags = flags();
    setFlags(currentFlags & (~Qt::ItemIsSelectable));
    parent->roots[type] = this;
    parent->addTopLevelItem(this);
    is_root_item = true;
}

object_tree_item::object_tree_item(std::string name, network_objects type, QGraphicsItem *graph_item, int id, objects_tree *parent) : QTreeWidgetItem()
{
    setText(0, QString::fromStdString(name));
    this->name = name;
    parent->roots[type]->addChild(this);
    this->graph_item = graph_item;
    this->id = id;
    is_root_item = false;
    is_item_link = type == network_objects::link;
}

std::string object_tree_item::get_name()
{
    return name;
}

vertex *object_tree_item::get_object()
{
    it *item = dynamic_cast<it *>(graph_item);

    return item->v;
}