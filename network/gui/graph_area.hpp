#include <functional>

#include "scene.hpp"
#include "../kernel/topology.hpp"
#include "../writer/writer.hpp"
#include "result_widget.hpp"
// #include "../reader/topology_reader.hpp"

#pragma once

it *source_creator(vertex *, QWidget *);

it *sink_creator(vertex *, QWidget *);

it *pipe_creator(vertex *, QWidget *);

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
    std::map<link_id, QGraphicsLineItem *> links;

private: // constants
    static constexpr std::pair<int, int> object_size = {20, 20};
    static constexpr std::pair<int, int> default_coord = {-100, -100};
    std::map<network_objects, QColor> object_color;
    std::map<std::string, std::function<it *(vertex *, QWidget *)>> item_creators;

public:
    QTabWidget *tabs;
    result_widget *results_view;

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

        gv->resize(this->width(), this->height());
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

        tabs->insertTab(0, gv, "Scheme");
        tabs->insertTab(1, results_view, "Results Table");

        // set creators
        item_creators["SOURCE"] = source_creator;
        item_creators["SINK"] = sink_creator;
        item_creators["PIPE"] = pipe_creator;
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

public slots: // link
    void add_link_pushed()
    {
        scene->link_add_flag = true;
    }

    void add_link(vertex *f, vertex *s, QGraphicsLineItem *link_line)
    {
        link *l = topology->create_link(f->get_id(), s->get_id());
        rep->print_message("Link between " + f->get_data()->get_name() + " and " + s->get_data()->get_name() + " successfully created");

        std::string link_name = "Link: " + f->get_data()->get_name() + " and " + s->get_data()->get_name();
        objects_list->add_object(network_objects::link, link_name, link_line);
        links[l->get_id()] = link_line;
    }

    void object_updated()
    {
        emit update_active_objects();
    }

public:
    void add_object(const std::string type, QPoint coords = QPoint(default_coord.first, default_coord.second), std::string data_file = "")
    {
        if (topology == nullptr)
        {
            rep->print_error("Cannot create " + type + ". Make network topology first");
            return;
        }

        vertex *v;

        error ret = topology->make_object(type, data_file, &v);

        if (!ret.is_ok())
        {
            rep->print_error(string(ret) + " in file " + data_file);
            return;
        }

        it *item = item_creators.at(type)(v, this);

        item->setPos(coords.x(), coords.y());
        scene->addItem(item);

        items[v->get_id()] = item;

        std::string object_name = v->get_data()->get_name();
        objects_list->add_object(string_to_type(type), object_name, item);

        gv->centerOn(item);
    }

private:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        // gv->resize(9 * this->width() / 10, this->height());
        gv->setGeometry(width() * 0.2, 0, width() * 0.7 - 1, height());
        results_view->setGeometry(width() * 0.2, 0, width() * 0.7 - 1, height());
        start_button->setGeometry(width() * 0.9, 0, width() * 0.1 - 1, 40);
        button->setGeometry(width() * 0.9, 41, width() * 0.1 - 1, 40);
        link_button->setGeometry(width() * 0.9, 82, width() * 0.1 - 1, 40);
        objects_list->setGeometry(0, 0, width() * 0.2, height());
        tabs->setGeometry(width() * 0.2, 5, width() * 0.7 - 10, height());
        // scene->setSceneRect(this->width() / 10, this->width() / 10, 9 * this->width() / 10, this->height());
    }

signals:
    void update_active_objects();
};
