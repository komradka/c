#include "graph_area.hpp"
#include "../managers/nd_manager.h"
#include "nd_con/async_reporter.h"
#include "nd_con/tasks/CALCULATE_task.h"

it *source_creator(vertex *v, QWidget *gui_manager)
{
    source_item *item = new source_item(v, 0, gui_manager);

    return item;
}

it *sink_creator(vertex *v, QWidget *gui_manager)
{
    sink_item *item = new sink_item(v, 0, gui_manager);

    return item;
}

it *pipe_creator(vertex *v, QWidget *gui_manager)
{
    pipe_item *item = new pipe_item(v, 0, gui_manager);

    return item;
}

it *joint_creator(vertex *v, QWidget *gui_manager)
{
    joint_item *item = new joint_item(v, 0, gui_manager);

    return item;
}

error graph_area::add_object(const std::string type, QPoint coords, std::string data_file)
{
    vertex *v;

    RETURN_IF_FAIL(pm->create_network_object(type, data_file, &v));

    it *item = item_creators.at(type)(v, this);

    item->setPos(coords.x(), coords.y());
    scene->addItem(item);

    items[v->get_id()] = item;

    std::string object_name = v->get_data()->get_name();
    objects_list->add_object(string_to_type(type), object_name, v->get_id(), item);

    gv->centerOn(item);

    return error(OK);
}

void graph_area::add_link(std::string name1, std::string name2)
{
    object_id obj1 = pm->get_object_by_name(name1);
    object_id obj2 = pm->get_object_by_name(name2);

    it *item1 = items.at(obj1);
    it *item2 = items.at(obj2);

    scene->draw_link(item1, item2);
}

void graph_area::add_link(vertex *f, vertex *s, link_item *link_line)
{
    link *l;

    pm->add_link(f->get_id(), s->get_id(), &l);

    std::string link_name = "Link: " + f->get_data()->get_name() + " and " + s->get_data()->get_name();
    objects_list->add_object(network_objects::link, link_name, l->get_id(), link_line);
    links[l->get_id()] = link_line;
}

void graph_area::delete_link(int id)
{
    std::pair<object_id, object_id> connected_objs = pm->get_connected_object(id);

    pm->delete_link(id);

    QGraphicsLineItem *link_item = links.at(id);

    scene->delete_link(link_item, {items.at(connected_objs.first), items.at(connected_objs.second)});

    links.erase(id);
}

void graph_area::delete_object(int id)
{
    std::vector<link_id> connected_links;
    std::vector<object_id> connected_objects;

    pm->delete_object(id, &connected_objects, &connected_links);

    for (auto link : connected_links)
    {
        links.erase(link);
        objects_list->delete_link_item(link);
    }

    std::vector<it *> connected_items;
    connected_items.resize(connected_objects.size());

    for (unsigned int i = 0; i < connected_objects.size(); i++)
    {
        connected_items[i] = items.at(connected_objects[i]);
    }

    scene->delete_object(items.at(id), connected_items);
    items.erase(id);
}

void graph_area::add_link_pushed()
{
    scene->link_add_flag = true;
}

void graph_area::try_start_calculation ()
{
  pm->add_task (new CALCULATE_task (*pm));
}
