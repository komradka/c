#include "graph_area.hpp"
#include "nd/iface/nd_manager.h"
#include "nd/iface/async_reporter.h"
#include "nd/tasks/CALCULATE_task.h"

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

graph_area::graph_area (QWidget *parent, QObject *manager) : QWidget(parent)
{
    tabs = new QTabWidget(this);
    scene = new Scene(this);
    gv = new QGraphicsView(this);
    button = new QPushButton(this);
    link_button = new QPushButton(this);
    start_button = new QPushButton(this);
    add_object_menu = new QMenu(this);
    objects_list = new objects_tree(this);
    results_view = new result_widget(objects_list);

    connect(link_button, SIGNAL(clicked()), this, SLOT(add_link_pushed()));
    //connect(start_button, SIGNAL(clicked()), manager, SLOT(try_start_calculation()));
    connect(start_button, SIGNAL(clicked()), this, SLOT(try_start_calculation()));

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->set_items_pointer(&items, &links);

    // gv->resize(this->width(), this->height());
    gv->setSceneRect(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
    gv->setScene(scene);
    gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    gv->setDragMode(QGraphicsView::ScrollHandDrag);

    start_button->setText("Calculate");

    link_button->setText("Add Link");

    button->setText("Add Object");
    button->setMenu(add_object_menu);
    add_object_menu->addAction("Add Source", this, SLOT(add_source()));
    add_object_menu->addAction("Add Sink", this, SLOT(add_sink()));
    add_object_menu->addAction("Add Pipe", this, SLOT(add_pipe()));
    add_object_menu->addAction("Add Joint", this, SLOT(add_joint()));

    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(objects_list);
    splitter->addWidget(tabs);

    tabs->insertTab(0, gv, "Scheme");
    tabs->insertTab(1, results_view, "Results Table");

    // set creators
    item_creators["SOURCE"] = source_creator;
    item_creators["SINK"] = sink_creator;
    item_creators["PIPE"] = pipe_creator;
    item_creators["JOINT"] = joint_creator;
}

graph_area::~graph_area()
{
    // delete scene;
    // delete gv;
    // delete add_object_menu;
    // delete button;
    // delete objects_list;

    // for (auto obj : items)
    // {
    //     delete obj.second;
    // }
}

void graph_area::make_pipe_project()
{
    // if (pipes != nullptr)
    // {
    //     rep->print_warning("Pipe project already exist");
    //     return;
    // }
    // if (fluid_view == nullptr)
    // {
    //     return;
    // }

    // pipes = new pipe_project(fluid_view);
    // tabs->insertTab(tabs_cout, pipes, "Pipe Project");
    // tabs_cout++;
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

void graph_area::change_selection_items()
{
    QTreeWidgetItemIterator iterator(objects_list, QTreeWidgetItemIterator::All);

    while (*iterator)
    {
        auto it = *iterator;
        object_tree_item *si = dynamic_cast<object_tree_item *>(it);
        if (si == nullptr)
        {
            iterator++;
            continue;
        }
        int id = si->get_id();
        if (si->is_root())
        {
            iterator++;
            continue;
        }

        if (!si->is_link())
        {
            items.at(id)->is_clicked = it->isSelected();
            items.at(id)->update();
        }
        else
        {
            links.at(id)->is_clicked = it->isSelected();
            QLineF line = links.at(id)->line();
            links.at(id)->redrow(line);
        }
        iterator++;
    }
}

void graph_area::try_start_calculation ()
{
  pm->add_task (new CALCULATE_task (*pm));
}

void graph_area::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    splitter->setGeometry(0, 0, width() * 0.9, height());
    start_button->setGeometry(width() * 0.9, 0, width() * 0.1 - 1, 40);
    button->setGeometry(width() * 0.9, 41, width() * 0.1 - 1, 40);
    link_button->setGeometry(width() * 0.9, 82, width() * 0.1 - 1, 40);
}

#include "moc_graph_area.cpp"
