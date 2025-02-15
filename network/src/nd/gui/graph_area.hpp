#include <functional>

#include "scene.hpp"
#include "nd/iface/topology.hpp"
#include "nd/writer/writer.hpp"
#include "result_widget.hpp"
#include "pvt/include/fluid_widget.hpp"

#include "enum_widget.hpp"
#include "pipe_project/pipe_project.hpp"

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
    int fluid_project_count = 0;
    result_widget *results_view;
    pipe_project *pipes = nullptr;

public:
    friend class writer;

    graph_area(reporter *rep, QWidget *parent, QObject *manager);
    ~graph_area();

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

    void copy_results(const std::map<object_id, phys_q> &object_res)
    {
        results_view->object_results = object_res;
    }

    void update_results()
    {
        results_view->reprint_table();
    }

    void make_fluid(fluid_widget *fluid, std::string name)
    {
        fluid_project_count++;
        tabs->insertTab(tabs_cout, fluid, QString::fromStdString(name));
        tabs_cout++;
    }

    void make_pipe_project();

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

public: // objects
    Q_SLOT void add_source()
    {
        add_object("SOURCE");
    }

    Q_SLOT void add_sink()
    {
        add_object("SINK");
    }

    Q_SLOT void add_pipe()
    {
        add_object("PIPE");
    }

    Q_SLOT void add_joint()
    {
        add_object("JOINT");
    }

public: // link
    Q_SLOT void change_selection_items();

    Q_SLOT void try_start_calculation();

    Q_SLOT void add_link_pushed();

    Q_SLOT void add_link(vertex *f, vertex *s, link_item *link_line);

    Q_SLOT void object_updated()
    {
        emit update_active_objects();
    }

    Q_SLOT void delete_object(int id);

    Q_SLOT void delete_link(int id);

public:
    error add_object(const std::string type, QPoint coords = QPoint(default_coord.first, default_coord.second), std::string data_file = "");

    void add_link(std::string name1, std::string name2);

private:
    void resizeEvent(QResizeEvent *event) override;

public:
    Q_SIGNAL void update_active_objects();
};
