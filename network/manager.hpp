#ifndef MANAGER
#define MANAGER

#include "main_window.hpp"
#include "reader/reader.hpp"
#include "gui/graph_area.hpp"
#include "writer/writer.hpp"
#include "manager_gui/save_dialog.hpp"
#include "manager_gui/load_dialog.hpp"
#include "manager_gui/settings_dialog.hpp"

#include "kernel/network_solver/nd_solver.hpp"
// #include "network_solver/nd_solver.hpp"

#define IS_DEBUG 1

class manager : public QObject
{
    Q_OBJECT

private:
    MWindow *main_window = nullptr;
    QAction *action = nullptr;
    QMenuBar *tool_bar = nullptr;
    QStatusBar *status_bar = nullptr;
    graph_area *window = nullptr;
    save_dialog *d_save = nullptr;
    load_dialog *d_load = nullptr;
    settings_dialog *d_setting = nullptr;
    reporter *rep;
    nd_solver *solver;

    reader file_reader;

private:
    graph *network_topology = nullptr;

    unsigned int results_count = 0;
    QString directory;

private:
    network_statistic *statistic;

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
            delete statistic;
        }
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
    }

    void make_window()
    {
        statistic = new network_statistic;
        main_window = new MWindow(this);
        tool_bar = new QMenuBar(main_window);
        status_bar = new QStatusBar(main_window);
        rep = new reporter(statistic, IS_DEBUG, status_bar);
        window = new graph_area(rep, main_window, this);
        d_setting = new settings_dialog(this);

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
    void open_settings_dialog()
    {
        d_setting->show();
    }

    void try_start_calculation()
    {
        if (network_topology == nullptr)
        {
            rep->print_error("Make network topology first");
            return;
        }
        solver = new nd_solver(this, rep, network_topology, d_setting);

        window->disable_calc_button();

        solver->prepare_network_and_try_start_calculation();
    }

    void calculation_end()
    {
        if (solver->calculation_ret.is_ok())
        {
            window->copy_results(solver->get_object_results());

            window->update_results();
        }
        else 
        {
            rep->print_error(solver->calculation_ret);
        }

        window->enable_calc_button();
    }

    void change_results()
    {
        result_item *res_item = dynamic_cast<result_item *>(d_load->result_tree->selectedItems()[0]);
        result_info res = res_item->result;
        d_load->close();
        delete d_load;
        d_load = nullptr;

        if (network_topology != nullptr)
            delete network_topology;
        make_network();

        rep->print_message("Loading - " + res.res_name);
        error ret = file_reader.read_topology(res.gui_dir + "/GUI.data", window);
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
    }

    void make_network()
    {
        if (network_topology != nullptr)
        {
            rep->print_warning("Network already existing");
            return;
        }

        network_topology = new graph(rep);

        rep->print_message("Network successfully created");

        window->set_network(network_topology);
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

        writer *w = new writer(directory.toStdString(), rep, window);
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

        action = tool_bar->addAction("&Make network", this, SLOT(make_network()));
        action->setShortcut(QString("Ctrl+2"));

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