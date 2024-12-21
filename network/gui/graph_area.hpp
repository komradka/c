#include <functional>

#include "scene.hpp"
#include "../kernel/topology.hpp"
#include "../writer/writer.hpp"
#include "result_widget.hpp"
#include "fluid_widget.hpp"

#include "../gui_utility/enum_widget.hpp"
#include "pipe_project/pipe_project.hpp"
// #include "../reader/topology_reader.hpp"

class nd_manager;

#pragma once

it *source_creator(vertex *, QWidget *);

it *sink_creator(vertex *, QWidget *);

it *pipe_creator(vertex *, QWidget *);

it *joint_creator(vertex *, QWidget *);

class graph_area : public QWidget
{
    Q_OBJECT

private:
    nd_manager *pm;

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

    void set_manager(nd_manager *pm)
    {
        this->pm = pm;
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

    void try_start_calculation ();

    void add_link_pushed();

    void add_link(vertex *f, vertex *s, link_item *link_line);

    void object_updated()
    {
        emit update_active_objects();
    }

    void delete_object(int id);

    void delete_link(int id);

public:
    error add_object(const std::string type, QPoint coords = QPoint(default_coord.first, default_coord.second), std::string data_file = "");

    void add_link(std::string name1, std::string name2);

private:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        splitter->setGeometry(0, 0, width() * 0.9, height());
        start_button->setGeometry(width() * 0.9, 0, width() * 0.1 - 1, 40);
        button->setGeometry(width() * 0.9, 41, width() * 0.1 - 1, 40);
        link_button->setGeometry(width() * 0.9, 82, width() * 0.1 - 1, 40);
    }

signals:
    void update_active_objects();
};
