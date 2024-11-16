#include <iostream>
#include <fstream>
#include <filesystem>

#include "../error.hpp"
#include "../reporter.hpp"

using namespace std;

#pragma once

enum class file_errors
{
    exist = -10,
    open = -11,
    make_dir = -12,
};

class graph_area;
class vertex;

class writer
{
private:
    unsigned int res_num;
    string dir;
    ofstream out;
    graph_area *storage = nullptr;
    reporter *rep;

public:
    writer(string dir, reporter *r, graph_area *window)
    {
        rep = r;
        this->dir = dir;
        storage = window;
    }

    ~writer()
    {
        if (out.is_open())
            out.close();
    }

    error write_network_to_file(unsigned int res_num, QString res_name)
    {
        this->res_num = res_num;

        string main_file = dir + "/project.np";
        bool exist = std::filesystem::exists(main_file);

        if (exist && res_num == 0)
            return error("File already exists", (int)file_errors::exist);

        out.open(main_file, std::ios::app);

        if (!out.is_open())
        {
            return error("Cannot open file", (int)file_errors::open);
        }

        if (exist)
            out << endl;

        out << "RES_NUM " << res_num << endl;
        out << "RES_NAME " << res_name.toStdString() << endl; 

        RETURN_IF_FAIL(write_gui());

        RETURN_IF_FAIL(write_results());

        return error(OK);
    }

private:
    error write_gui();

    error write_results();

    error write_object_data(string data_file, vertex *v);
};