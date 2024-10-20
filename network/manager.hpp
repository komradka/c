#ifndef MANAGER
#define MANAGER

#include "main_window.hpp"
#include "reader/reader.hpp"
#include "reporter.hpp"
#include "gui/graph_area.hpp"

class manager : public QObject
{
    Q_OBJECT

private:
    MWindow *main_window = nullptr;
    QAction *action = nullptr;
    QMenuBar *tool_bar = nullptr;
    QStatusBar *status_bar = nullptr;
    graph_area *window = nullptr;
    reporter *rep;

    reader file_reader;

private:
    graph *network_topology = nullptr;

public:
    ~manager()
    {
        if (main_window != nullptr)
        {
            delete tool_bar;
            delete action;
            delete status_bar;
            delete main_window;
            delete rep;
            delete window;
        }
        if (network_topology != nullptr)
        {
            delete network_topology;
        }
    }

    void make_window()
    {
        main_window = new MWindow(this);
        tool_bar = new QMenuBar(main_window);
        status_bar = new QStatusBar(main_window);
        rep = new reporter(status_bar);
        window = new graph_area(rep, main_window);

        tool_bar->setMaximumHeight(30);
        main_window->setMenuBar(tool_bar);
        main_window->setStatusBar(status_bar);
        main_window->setCentralWidget(window);
        status_bar->setFixedHeight(100);
        rep->setMinimumHeight(100);
        main_window->installEventFilter(this);

        

        show_window();
        add_action();
    }

    bool eventFilter(QObject *object, QEvent *event)
    {
        if (object == main_window && event->type() == QEvent::Resize)
        {
            rep->resize(main_window->width() - 1, main_window->height() / 10);
        }
        return false;
    }

public slots:
    void make_fluid()
    {
    }

    void make_network()
    {
        if (network_topology != nullptr)
            delete network_topology;

        network_topology = new graph(rep);

        rep->print_message("Network successfully created");

        window->set_network(network_topology);
        window->update();
    }

private:
    void show_window()
    {
        main_window->show();
    }
    void add_action()
    {
        action = tool_bar->addAction("&Exit", main_window, SLOT(close()));
        action->setShortcut(QString("Ctrl+X"));

        action = tool_bar->addAction("&Make fluid", this, SLOT(make_fluid()));
        action->setShortcut(QString("0"));

        action = tool_bar->addAction("&Make network", this, SLOT(make_network()));
        action->setShortcut(QString("1"));
    }
};

#endif