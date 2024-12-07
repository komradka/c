#include <functional>

#include "scene.hpp"
#include "../kernel/topology.hpp"
#include "../writer/writer.hpp"
#include "result_widget.hpp"
#include "fluid_widget.hpp"

#include "../gui_utility/enum_widget.hpp"
#include "pipe_project/pipe_project.hpp"
// #include "../reader/topology_reader.hpp"

#pragma once

it *source_creator(vertex *, QWidget *);

it *sink_creator(vertex *, QWidget *);

it *pipe_creator(vertex *, QWidget *);

it *joint_creator(vertex *, QWidget *);

class graph_area : public QWidget
{
    Q_OBJECT

private:
    graph *topology = nullptr;

    Scene *scene;
    QGraphicsView *gv;
    QPushButton *button;
    QPushButton *link_button;
    QPushButton *start_button;
    QMenu *add_object_menu;
    objects_tree *objects_list;

    reporter *rep;

    std::map<object_id, it *> items;
    std::map<link_id, link_item *> links;

private: // constants
    static constexpr std::pair<int, int> object_size = {20, 20};
    static constexpr std::pair<int, int> default_coord = {-100, -100};
    std::map<network_objects, QColor> object_color;
    std::map<std::string, std::function<it *(vertex *, QWidget *)>> item_creators;

public:
    QSplitter *splitter;
    QVBoxLayout *button_layout;
    QTabWidget *tabs;
    int tabs_cout = 2;
    result_widget *results_view;
    fluid_widget *fluid_view = nullptr;
    pipe_project *pipes = nullptr;

public:
    friend class writer;

    graph_area(reporter *rep, QWidget *parent, QObject *manager) : QWidget(parent)
    {
        this->rep = rep;

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
        connect(start_button, SIGNAL(clicked()), manager, SLOT(try_start_calculation()));

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
    ~graph_area()
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

    void set_network(graph *topology)
    {
        this->topology = topology;
        connect(this, SIGNAL(update_active_objects()), topology, SLOT(update_active_objects()));
    }

    void disable_calc_button()
    {
        start_button->setEnabled(false);
    }

    void enable_calc_button()
    {
        start_button->setEnabled(true);
    }

    void copy_results(std::map<object_id, phys_q> &object_res)
    {
        results_view->object_results = object_res;
    }

    void update_results()
    {
        results_view->reprint_table();
    }

    void make_fluid()
    {
        if (fluid_view != nullptr)
        {
            rep->print_warning("Fluid already exist");
            return;
        }

        fluid_view = new fluid_widget();

        tabs->insertTab(tabs_cout, fluid_view, "Water Props");
        tabs_cout++;
    }

    void make_pipe_project()
    {
        if (pipes != nullptr)
        {
            rep->print_warning("Pipe project already exist");
            return;
        }
        if (fluid_view == nullptr)
        {
            return;
        }

        pipes = new pipe_project(fluid_view);
        tabs->insertTab(tabs_cout, pipes, "Pipe Project");
        tabs_cout++;
    }

    QSize minimumSizeHint() const
    {
        return QSize(600, 100);
    }
    QSize maximumSize() const
    {
        return QSize(2000, 2000);
    }
    QSize sizeHint() const
    {
        return QSize(1000, 1000);
    }

public slots: // objects
    void add_source()
    {
        add_object("SOURCE");
    }

    void add_sink()
    {
        add_object("SINK");
    }

    void add_pipe()
    {
        add_object("PIPE");
    }

    void add_joint()
    {
        add_object("JOINT");
    }

public slots: // link
    void change_selection_items()
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

    void add_link_pushed()
    {
        scene->link_add_flag = true;
    }

    void add_link(vertex *f, vertex *s, link_item *link_line)
    {
        link *l = topology->create_link(f->get_id(), s->get_id());
        rep->print_message("Link between " + f->get_data()->get_name() + " and " + s->get_data()->get_name() + " successfully created");

        std::string link_name = "Link: " + f->get_data()->get_name() + " and " + s->get_data()->get_name();
        objects_list->add_object(network_objects::link, link_name, l->get_id(), link_line);
        links[l->get_id()] = link_line;
    }

    void object_updated()
    {
        emit update_active_objects();
    }

    void delete_object(int id)
    {
        std::string name = topology->get_object_name(id);
        rep->print_message("Object " + name + " deleted");

        std::vector<object_id> connected_objects = topology->get_neighbors(id);

        for (auto obj : connected_objects)
        {
            links.erase(topology->get_link_between_object(id, obj).value());
            objects_list->delete_link_item(obj);
        }

        topology->delete_object(id);

        std::vector<it *> connected_items;
        connected_items.resize(connected_objects.size());

        for (unsigned int i = 0; i < connected_objects.size(); i++)
        {
            connected_items[i] = items.at(connected_objects[i]);
        }

        scene->delete_object(items.at(id), connected_items);
        items.erase(id);
    }

    void delete_link(int id)
    {
        std::pair<object_id, object_id> connected_objs = topology->get_connected_object(id);
        rep->print_message("Link between " + topology->get_object_name(connected_objs.first) + " and " + topology->get_object_name(connected_objs.second) + " deleted");

        topology->delete_link(id);

        QGraphicsLineItem *link_item = links.at(id);

        scene->delete_link(link_item, {items.at(connected_objs.first), items.at(connected_objs.second)});

        links.erase(id);
    }

public:
    error add_object(const std::string type, QPoint coords = QPoint(default_coord.first, default_coord.second), std::string data_file = "")
    {
        if (topology == nullptr)
        {
            rep->print_error("Cannot create " + type + ". Make network topology first");
            return error("Cannot create " + type + ". Make network topology first");
        }

        vertex *v;

        error ret = topology->make_object(type, data_file, &v);

        if (!ret.is_ok())
        {
            rep->print_error(string(ret) + " in file " + data_file);
            return error(string(ret) + " in file " + data_file);
        }

        it *item = item_creators.at(type)(v, this);

        item->setPos(coords.x(), coords.y());
        scene->addItem(item);

        items[v->get_id()] = item;

        std::string object_name = v->get_data()->get_name();
        objects_list->add_object(string_to_type(type), object_name, v->get_id(), item);

        gv->centerOn(item);

        return error(OK);
    }

    void add_link(std::string name1, std::string name2)
    {
        auto obj1 = topology->get_object_by_name(name1);
        auto obj2 = topology->get_object_by_name(name2);

        if (!obj1.has_value() || !obj2.has_value())
        {
            return;
        }

        // link *l = topology->create_link(obj1.value(), obj2.value());

        // links[l->get_id()] = l;

        // rep->print_message("Link between " + f->get_data()->get_name() + " and " + s->get_data()->get_name() + " successfully created");

        it *item1 = items.at(obj1.value());
        it *item2 = items.at(obj2.value());

        scene->draw_link(item1, item2);
    }

private:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        // gv->resize(9 * this->width() / 10, this->height());
        splitter->setGeometry(0, 0, width() * 0.9, height());
        // gv->setGeometry(width() * 0.2, 0, width() * 0.7 - 1, height());
        // results_view->setGeometry(width() * 0.2, 0, width() * 0.7 - 1, height());
        start_button->setGeometry(width() * 0.9, 0, width() * 0.1 - 1, 40);
        button->setGeometry(width() * 0.9, 41, width() * 0.1 - 1, 40);
        link_button->setGeometry(width() * 0.9, 82, width() * 0.1 - 1, 40);
        // objects_list->setGeometry(0, 0, width() * 0.2, height());
        // tabs->setGeometry(width() * 0.2, 5, width() * 0.7 - 10, height());
    }

signals:
    void update_active_objects();
};
