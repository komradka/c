#include "project_button.hpp"
#include "../system_analyzer.hpp"

#pragma once

class Project_Window : public QMainWindow
{
    Q_OBJECT

private:
    project_manager *pm;

private:
    project_button *nd_button = nullptr;
    project_button *sim_button = nullptr;

    QLabel *thread_label;
    QComboBox *thread_box;

public:
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

    Project_Window(project_manager *pm)
    {
        this->pm = pm;

        setWindowTitle("Project");
        make_nd_button();
        make_sim_button();

        thread_label = new QLabel(this);
        thread_label->setText("Thread: ");
        thread_box = new QComboBox(this);
        fill_thread_box();
    }

    ~Project_Window()
    {
    }

public:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        if (nd_button)
            nd_button->setGeometry(width() / 2 + 1, 30, width() / 2 - 2, height() - 32);
        if (sim_button)
            sim_button->setGeometry(0, 30, width() / 2 - 2, height() - 32);

        thread_label->setGeometry(1, 1, 40, 28);
        thread_box->setGeometry(51, 1, 80, 28);
    }

private:
    void make_nd_button()
    {
        button_settings nd_button_settings;
        nd_button_settings.name = "Network\nSimulation";
        nd_button_settings.default_colour = "#87CEFA";
        nd_button_settings.hover_color = "#00BFFF";
        nd_button_settings.pos = 1;
        nd_button_settings.type = project_type::network;
        nd_button = new project_button(nd_button_settings, this);
    }

    void make_sim_button()
    {
        button_settings sim_button_settings;
        sim_button_settings.name = "Hydrodynamic\nSimulation";
        sim_button_settings.default_colour = "#F08080";
        sim_button_settings.hover_color = "#DC143C";
        sim_button_settings.pos = 0;
        sim_button_settings.type = project_type::simulator;
        sim_button = new project_button(sim_button_settings, this);
    }

    void fill_thread_box()
    {
        int max_thread = system_analyzer::get_thread_count();

        int curr_thread = 1;
        while (curr_thread < max_thread)
        {
            thread_box->addItem(QString("%1 core").arg(curr_thread));
            curr_thread *= 2;
        }

        thread_box->addItem(QString("%1 core").arg(max_thread));
    }

    int get_selected_thread_num()
    {
        return thread_box->currentText().split(" ")[0].toInt();
    }

public slots:
    void new_project(int type)
    {
        QString file = QFileDialog::getSaveFileName(this,
                                                    "Make Project",
                                                    QDir::currentPath(),
                                                    "Project (*.np);;All files (*.*)");

        error ret = pm->create_project((project_type)type, get_selected_thread_num(), file.toStdString());

        if (!ret.is_ok())
        {
            QMessageBox::warning(nullptr, "Creating project error", QString::fromStdString(ret.description()));
        }
    }

    void load_project(int type)
    {
        QString file = QFileDialog::getOpenFileName(this,
                                                    "Load Project",
                                                    QDir::currentPath(),
                                                    "Project (*.np)");

        error ret = pm->load_project((project_type)type, get_selected_thread_num(), file.toStdString());

        if (!ret.is_ok())
        {
            QMessageBox::warning(nullptr, "Load project error", QString::fromStdString(ret.description()));
        }
    }
};