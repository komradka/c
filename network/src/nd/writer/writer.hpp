#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

#include "error.hpp"
#include "common/reporters/include/report_system.hpp"
#include "nd/nd_io/nd_io_path.hpp"
#include "nd/nd_io/nd_io_kwords.hpp"

using namespace std;

#pragma once

class settings_dialog;

enum class file_errors
{
    exist = -10,
    open = -11,
    make_dir = -12,
};

class graph_area;
class vertex;
class graph;

class writer
{
private:
    nd_io_path paths;
    nd_kwords kwords;

    unsigned int res_num;
    std::string res_name;
    string project_dir;
    string project;
    std::string result_dir;
    std::string main_project_file;
    std::string gui_dir;

    graph *topology = nullptr;
    graph_area *storage = nullptr;
    settings_dialog *settings = nullptr;
    report_system *rep;

    ofstream project_out;
    ofstream settings_out;
    ofstream gui_out;
    ofstream pvt_out;

public:
    writer(string project_name, report_system *r, graph *topology, graph_area *window, settings_dialog *settings)
    {
        rep = r;

        auto found = project_name.find_last_of("/\\");
        project_dir = project_name.substr(0, found);
        project = project_name.substr(found + 1);

        std::replace(project.begin(), project.end(), ' ', '_');
        storage = window;
        this->settings = settings;
        this->topology = topology;
    }

    ~writer()
    {
        if (project_out.is_open())
            project_out.close();

        if (gui_out.is_open())
            gui_out.close();

        if (settings_out.is_open())
            settings_out.close();

        if (pvt_out.is_open())
            pvt_out.close();
    }

    error write_network_to_file(unsigned int res_num, QString res_name)
    {
        this->res_num = res_num;
        this->res_name = res_name.toStdString();

        bool exist = false;

        RETURN_IF_FAIL (prepare(exist));

        if (exist)
            project_out << endl;

        project_out << kwords.res_num << " " << res_num << endl;
        project_out << kwords.res_name << " " << res_name.toStdString() << endl;

        RETURN_IF_FAIL(write_gui());
        RETURN_IF_FAIL(write_settings());
        RETURN_IF_FAIL(write_results());

        return error(OK);
    }

private:
    error prepare(bool &);

    error write_gui();
    error write_gui_object();

    error write_settings();

    error write_results();

    error write_object_data(string data_file, vertex *v);
};
