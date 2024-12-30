#include "nd_window.hpp"
#include "../managers/nd_manager.h"

nd_main_window::nd_main_window(std::string name)
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

void nd_main_window::closeEvent(QCloseEvent *event)
{
    pm->project_closed();
    event->accept();
}

void nd_main_window::open_settings_dialog()
{
    pm->show_settings();
}

void nd_main_window::make_fluid()
{
    window->make_fluid(pm->create_fluid());
}

error nd_main_window::save_project()
{
    d_save = new save_dialog(pm->get_results_count(), this);
    d_save->show();

    return error(OK);
    // return ;
}

void nd_main_window::save_project_click()
{
    QString res_name = d_save->get_result_name();
    d_save->close();
    delete d_save;
    d_save = nullptr;

    pm->save_project(res_name.toStdString());
}

void nd_main_window::load_project(std::vector<result_info> &results)
{
    d_load = new load_dialog(results, this);
    d_load->show();
}

void nd_main_window::change_results()
{
    result_item *res_item = dynamic_cast<result_item *>(d_load->result_tree->selectedItems()[0]);
    result_info res = res_item->result;
    d_load->close();
    delete d_load;
    d_load = nullptr;

    pm->load_project(res);
}

void nd_main_window::set_workflow()
{
    wf = new workflow_dialog(pm->get_workflow());
}

void nd_main_window::open_workflow()
{
    wf->show();
}

