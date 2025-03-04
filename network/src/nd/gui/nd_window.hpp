#include <string>
#include <iostream>

#include "common_gui/include/report_widget.hpp"
#include "common/os/include/network_statistic.hpp"
#include "nd/gui/graph_area.hpp"
#include "nd/gui/manager_gui/save_dialog.hpp"
#include "nd/gui/manager_gui/load_dialog.hpp"
#include "nd/gui/manager_gui/fluid/fluid_dialog.hpp"
#include "workflow_dialog.hpp"

class nd_manager;

#pragma once

class nd_main_window : public QMainWindow
{
    Q_OBJECT

private:
    QStatusBar *statusbar;
    QSplitter *splitter;
    report_widget *wrep;
    network_statistic *statistic;
    graph_area *window = nullptr;
    QMenuBar *tool_bar = nullptr;
    QAction *action = nullptr;

    nd_manager *pm = nullptr;
    save_dialog *d_save;
    load_dialog *d_load;
    fluid_dialog *d_fluid;

    workflow_dialog *wf;

    report_system *rep;

public:
    friend class nd_manager;

    QSize minimumSizeHint() const
    {
        return QSize(600, 100);
    }
    QSize sizeHint() const
    {
        return QSize(1000, 1000);
    }

    nd_main_window(std::string name);

    bool eventFilter(QObject *object, QEvent *event)
    {
        if (object == statusbar && event->type() == QEvent::Resize)
        {
            wrep->setGeometry(0, 0, statusbar->width(), statusbar->height());
        }
        return false;
    }

    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event)
        splitter->setGeometry(0, 0, width(), height());
    }

    report_widget *get_wrep()
    {
        return wrep;
    }

    void set_reporter(report_system *rep)
    {
        this->rep = rep;
        window->set_reporter(rep);
    }

    void set_pm(nd_manager *pm)
    {
        this->pm = pm;
        window->set_manager(pm);
    }

    void set_workflow();

    void copy_results (const std::map<object_id, phys_q> &object_res)
    {
        window->copy_results(object_res);
        window->enable_calc_button();
    }

    void closeEvent(QCloseEvent *event);

    graph_area *get_gui_manager()
    {
        return window;
    }

    void load_project(std::vector<result_info> &results);

    void make_fluid(std::string name);

    void set_fluid(int id);

public: // actions
    Q_SLOT error save_project();

    Q_SLOT void open_settings_dialog();

    Q_SLOT void open_fluid_dialog();

    Q_SLOT void save_project_click();

    Q_SLOT void change_results();

    Q_SLOT void open_workflow();

private:
    void add_action()
    {
        action = tool_bar->addAction("&Exit", this, SLOT(close()));
        action->setShortcut(QString("Ctrl+X"));

        action = tool_bar->addAction("&Settings", this, SLOT(open_settings_dialog()));
        action->setShortcut(QString("Ctrl+0"));

        action = tool_bar->addAction("&PVT", this, SLOT(open_fluid_dialog()));
        action->setShortcut(QString("Ctrl+1"));

        action = tool_bar->addAction("&Workflow", this, SLOT(open_workflow()));
        action->setShortcut(QString("Ctrl+W"));

        action = tool_bar->addAction("&Save project", this, SLOT(save_project()));
        action->setShortcut(QString("Ctrl+S"));
    }
};
