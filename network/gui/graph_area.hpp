#include <functional>

#include "scene.hpp"
#include "../kernel/topology.hpp"
// #include "../reader/topology_reader.hpp"

#pragma once

it *source_creator(vertex *);

it *sink_creator(vertex *);

class graph_area : public QWidget
{
    Q_OBJECT

private:
    graph *topology = nullptr;

    Scene *scene;
    QGraphicsView *gv;
    QPushButton *button;
    QPushButton *link_button;
    QMenu *add_object_menu;
    QListWidget *objects_list;

    reporter *rep;

    std::map<object_id, it *> items;

private: // constants
    const std::pair<int, int> object_size = {20, 20};
    const std::pair<int, int> default_coord = {-100, -100};
    std::map<network_objects, QColor> object_color;
    std::map<std::string, std::function<it *(vertex *)>> item_creators;

public:
    graph_area(reporter *rep, QWidget *parent) : QWidget(parent)
    {
        this->rep = rep;

        scene = new Scene(this);
        gv = new QGraphicsView(this);
        button = new QPushButton(this);
        link_button = new QPushButton(this);
        add_object_menu = new QMenu(this);
        objects_list = new QListWidget(this);

        connect(link_button, SIGNAL(clicked()), this, SLOT(add_link_pushed()));

        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        scene->set_items_pointer(&items);

        gv->resize(this->width(), this->height());
        gv->setSceneRect(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
        gv->setScene(scene);
        gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        gv->setDragMode(QGraphicsView::ScrollHandDrag);

        link_button->setText("Add Link");

        button->setText("Add Object");
        button->setMenu(add_object_menu);
        add_object_menu->addAction("Add Source", this, SLOT(add_source()));
        add_object_menu->addAction("Add Sink", this, SLOT(add_sink()));

        // visible_area = {{0, this->width()}, {0, this->height()}};

        // set creators
        item_creators["SOURCE"] = source_creator;
        item_creators["SINK"] = sink_creator;
    }
    ~graph_area()
    {
        delete scene;
        delete gv;
        delete add_object_menu;
        delete button;
        delete objects_list;

        for (auto obj : items)
        {
            delete obj.second;
        }
    }

    void set_network(graph *topology)
    {
        this->topology = topology;
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

public slots: // link
    void add_link_pushed()
    {
        scene->link_add_flag = true;
    }

    void add_link(vertex *f, vertex *s)
    {
        topology->create_link(f->get_id(), s->get_id());
        rep->print_message("Link between " + f->get_data()->get_name_for_gui() + " and " + s->get_data()->get_name_for_gui() + " successfully created");
    }

private:
    void add_object(const std::string type)
    {
        if (topology == nullptr)
        {
            rep->print_error("Cannot create " + type + ". Make network topology first");
            return;
        }

        QString str = QFileDialog::getOpenFileName();
        if (str.isEmpty())
        {
            rep->print_warning("User canceled");
            return;
        }
        rep->print_message("Opening - " + str.toStdString());

        vertex *v;

        error ret = topology->make_object(type, str.toStdString(), &v);

        if (!ret.is_ok())
        {
            rep->print_error(string(ret) + " in file " + str.toStdString());
        }

        it *item = item_creators.at(type)(v);

        item->setPos(default_coord.first, default_coord.second);
        scene->addItem(item);

        items[v->get_id()] = item;

        gv->centerOn(item);
    }

    void resizeEvent(QResizeEvent *event) override
    {
        // gv->resize(9 * this->width() / 10, this->height());
        gv->setGeometry(width() * 0.1, 0, width() * 0.8 - 1, height());
        button->setGeometry(width() * 0.9, 0, width() * 0.1 - 1, 40);
        link_button->setGeometry(width() * 0.9, 41, width() * 0.1 - 1, 40);
        objects_list->setGeometry(0, 0, width() * 0.1, height());
        // scene->setSceneRect(this->width() / 10, this->width() / 10, 9 * this->width() / 10, this->height());
    }
};
