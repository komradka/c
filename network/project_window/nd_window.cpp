#include "nd_window.hpp"
#include "../managers/nd_manager.h"

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