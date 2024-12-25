#include <string>
#include <iostream>

#include "../reporter.hpp"
#include "../gui/graph_area.hpp"
#include "../manager_gui/save_dialog.hpp"

class nd_manager;

#pragma once

class nd_main_window : public QMainWindow
{
    Q_OBJECT

private:
    QStatusBar *statusbar;
    QSplitter *splitter;
    reporter *rep;
    network_statistic *statistic;
    graph_area *window = nullptr;
    QMenuBar *tool_bar = nullptr;
    QAction *action = nullptr;

    nd_manager *pm;
    save_dialog *d_save;

public:
    QSize minimumSizeHint() const
    {
        return QSize(600, 100);
    }
    QSize sizeHint() const
    {
        return QSize(1000, 1000);
    }

    nd_main_window(std::string name)
    {
        setWindowTitle(QString::fromStdString(name));

        statusbar = new QStatusBar(this);

        statistic = new network_statistic;
        rep = new reporter(statistic, 1, statusbar);
        rep->setMinimumHeight(100);
        window = new graph_area(rep, this, this);
        tool_bar = new QMenuBar(this);
        tool_bar->setMaximumHeight(17);
        tool_bar->setMinimumHeight(17);

        setMenuBar(tool_bar);
        setCentralWidget(window);
        setStatusBar(statusbar);

        splitter = new QSplitter(Qt::Vertical, this);
        splitter->addWidget(tool_bar);
        splitter->addWidget(window);
        splitter->addWidget(statusbar);

        statusbar->installEventFilter(this);

        add_action();

        rep->print_message("Project " + name + " created");
    }

    bool eventFilter(QObject *object, QEvent *event)
    {
        if (object == statusbar && event->type() == QEvent::Resize)
        {
            rep->setGeometry(0, 0, statusbar->width(), statusbar->height());
        }
        return false;
    }

    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event)
        splitter->setGeometry(0, 0, width(), height());
    }

    reporter *get_reporter()
    {
        return rep;
    }

    void set_pm(nd_manager *pm)
    {
        this->pm = pm;
        window->set_manager(pm);
    }

    void closeEvent(QCloseEvent *event);

    graph_area *get_gui_manager()
    {
        return window;
    }

public slots: // actions
    error save_project();

    void open_settings_dialog();

    void make_fluid();

    void save_project_click();

private:
    void add_action()
    {
        action = tool_bar->addAction("&Exit", this, SLOT(close()));
        action->setShortcut(QString("Ctrl+X"));

        action = tool_bar->addAction("&Settings", this, SLOT(open_settings_dialog()));
        action->setShortcut(QString("Ctrl+0"));

        action = tool_bar->addAction("&Make fluid", this, SLOT(make_fluid()));
        action->setShortcut(QString("Ctrl+1"));

        action = tool_bar->addAction("&Save project", this, SLOT(save_project()));
        action->setShortcut(QString("Ctrl+S"));
    }
};