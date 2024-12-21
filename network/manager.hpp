#ifndef MANAGER
#define MANAGER

#include "main_window.hpp"
#include "reader/reader.hpp"
#include "gui/graph_area.hpp"
#include "writer/writer.hpp"
#include "manager_gui/save_dialog.hpp"
#include "manager_gui/load_dialog.hpp"
#include "manager_gui/settings_dialog.hpp"
#include "manager_gui/create_project_dialog.hpp"

#include "kernel/network_solver/nd_solver.hpp"
// #include "network_solver/nd_solver.hpp"

#include "nd_con/async_reporter.h"

#define IS_DEBUG 1

class manager : public QWidget
{
    Q_OBJECT

private:
    QSplitter *splitter;
    MWindow *main_window = nullptr;
    QAction *action = nullptr;
    QMenuBar *tool_bar = nullptr;
    QStatusBar *status_bar = nullptr;
    graph_area *window = nullptr;
    save_dialog *d_save = nullptr;
    load_dialog *d_load = nullptr;
    create_project_dialog *project_dialog = nullptr;
    settings_dialog *d_setting = nullptr;
    reporter *rep;
    nd_solver *solver;

    reader file_reader;

private:
    graph *network_topology = nullptr;

    unsigned int results_count = 0;
    QString directory;

    QString project_name;

private:
    network_statistic *statistic;

public:
    ~manager()
    {
        // if (main_window != nullptr)
        // {
        //     delete tool_bar;
        //     delete action;
        //     delete status_bar;
        //     delete main_window;
        //     delete rep;
        //     delete window;
        //     delete statistic;
        // }
        if (network_topology != nullptr)
        {
            delete network_topology;
        }
        if (d_save != nullptr)
        {
            delete d_save;
        }
        if (d_load != nullptr)
        {
            delete d_load;
        }
        if (project_dialog != nullptr)
        {
            delete project_dialog;
        }
    }

    void make_window()
    {
        main_window = new MWindow(this);
        status_bar = new QStatusBar(main_window);
        statistic = new network_statistic;
        rep = new reporter(statistic, IS_DEBUG, main_window);
        window = new graph_area(rep, main_window, this);
        tool_bar = new QMenuBar(main_window);

        main_window->setMenuBar(tool_bar);
        main_window->setCentralWidget(window);
        main_window->setStatusBar(status_bar);

        rep->setMinimumHeight(100);
        tool_bar->setMaximumHeight(17);
        tool_bar->setMinimumHeight(17);

        splitter = new QSplitter(Qt::Vertical, main_window);
        splitter->addWidget(tool_bar);
        splitter->addWidget(window);
        splitter->addWidget(status_bar);

        d_setting = new settings_dialog(this);

        main_window->installEventFilter(this);
        status_bar->installEventFilter(this);

        show_window();
        add_action();
    }

    settings_dialog *get_settings ()
    {
      return d_setting;
    }

    void set_settings (settings_dialog *settings)
    {
      d_setting = settings;
    }

    void print_log (message_t to_print)
    {
      switch (to_print.type)
        {
        case msg_types::error:
          rep->print_error (to_print.message);
          break;
        case msg_types::message:
          rep->print_message (to_print.message);
          break;
        case msg_types::warning:
          rep->print_warning (to_print.message);
          break;
        case msg_types::EMPTY:
          break;
        }
    }

    bool eventFilter(QObject *object, QEvent *event)
    {
        if (object == main_window && event->type() == QEvent::Resize)
        {
            splitter->setGeometry(0, 0, main_window->width(), main_window->height());
        }
        if (object == status_bar && event->type() == QEvent::Resize)
        {
            QRect r = status_bar->geometry();
            rep->setGeometry(0, r.top(), status_bar->width(), status_bar->height());
        }
        return false;
    }

public slots:
    void open_settings_dialog()
    {
        d_setting->show();
    }

    void copy_results (const std::map<object_id, phys_q> &object_res)
    {
        window->copy_results(object_res);
        window->enable_calc_button();
    }

    void change_results()
    {
        result_item *res_item = dynamic_cast<result_item *>(d_load->result_tree->selectedItems()[0]);
        result_info res = res_item->result;
        d_load->close();
        delete d_load;
        d_load = nullptr;

        project_name = QString::fromStdString(res.res_name);

        if (network_topology != nullptr)
            delete network_topology;
        make_network();

        rep->print_message("Loading - " + res.res_name);
        error ret = file_reader.read_data(res, window, d_setting);
        // 
        if (!ret.is_ok())
        {
            rep->print_error(ret);
        }
        else
        {
            network_topology->update_active_objects();
        }
    }

    void make_fluid()
    {
        window->make_fluid();
    }

    void make_pipe_project()
    {
        window->make_pipe_project();
    }

    void make_network()
    {
        if (network_topology != nullptr)
        {
            rep->print_warning("Network already existing");
            return;
        }

        if (project_name.isEmpty())
        {
            project_dialog = new create_project_dialog(this);
            project_dialog->show();
        }
        else
        {
            network_topology = new graph(rep);

            rep->print_message("Network successfully created");

            // window->set_network(network_topology);
            main_window->setWindowTitle(project_name);
            window->update();
        }
    }

    void crete_project_click()
    {
        project_name = project_dialog->get_result_name();
        delete project_dialog;
        project_dialog = nullptr;

        network_topology = new graph(rep);

        rep->print_message("Network successfully created");
        main_window->setWindowTitle(project_name);
        // window->set_network(network_topology);
        window->update();
    }

    error save_project()
    {
        if (network_topology == nullptr)
        {
            rep->print_error("Make network topology first");
            return error(OK);
        }
        rep->print_message("Saving...");

        if (results_count == 0)
        {
            directory = QFileDialog::getExistingDirectory();
            if (directory.isEmpty())
            {
                rep->print_warning("User canceled");
                return error(-1);
            }
        }

        d_save = new save_dialog(results_count, this);
        d_save->show();

        return error(OK);
    }

    void save_project_click()
    {
        QString res_name = d_save->get_result_name();
        d_save->close();
        delete d_save;
        d_save = nullptr;
        project_name = res_name;
        main_window->setWindowTitle(project_name);

        writer *w = new writer(directory.toStdString(), project_name.toStdString(), rep, window, d_setting);
        error ret = save_project_holder(w, res_name);
        if (!ret.is_ok())
        {
            rep->print_error(ret);
            delete w;
            return;
        }

        rep->print_message("Project saved in directory: " + directory.toStdString());
        results_count++;
        delete w;
    }

    void import_project()
    {
        if (network_topology != nullptr)
        {
            QMessageBox::warning(0, "Save project...", "Save project first!");

            if (!save_project().is_ok())
            {
                rep->print_error("Cannot import project. Save existing first.");
                return;
            }
        }

        directory = QFileDialog::getExistingDirectory();
        if (directory.isEmpty())
        {
            rep->print_warning("User canceled");
            return;
        }

        std::vector<result_info> results;
        file_reader.read_project(directory.toStdString(), results);

        results_count = results.size();
        d_load = new load_dialog(results, this);
        d_load->show();
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

        action = tool_bar->addAction("&Settings", this, SLOT(open_settings_dialog()));
        action->setShortcut(QString("Ctrl+0"));

        action = tool_bar->addAction("&Make fluid", this, SLOT(make_fluid()));
        action->setShortcut(QString("Ctrl+1"));

        action = tool_bar->addAction("&Make Project", this, SLOT(make_network()));
        action->setShortcut(QString("Ctrl+2"));

        action = tool_bar->addAction("&Make pipe project", this, SLOT(make_pipe_project()));
        action->setShortcut(QString("Ctrl+3"));

        action = tool_bar->addAction("&Save project", this, SLOT(save_project()));
        action->setShortcut(QString("Ctrl+S"));

        action = tool_bar->addAction("&Load project", this, SLOT(import_project()));
        action->setShortcut(QString("Ctrl+L"));
    }

    error save_project_holder(writer *w, QString res_name)
    {
        RETURN_IF_FAIL(w->write_network_to_file(results_count, res_name));
        return error(OK);
    }
};

#endif
