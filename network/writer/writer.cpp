#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

#include "writer.hpp"
#include "../gui/graph_area.hpp"

error writer::write_gui()
{
    error err(OK);

    QDir dir_manager;

    string dir_name = dir + "/GUI_" + to_string(res_num);

    if (dir_manager.exists(QString::fromStdString(dir_name)))
    {
        return error("Directory exists", (int)file_errors::exist);
    }

    if (dir_manager.mkdir(QString::fromStdString(dir_name)) == false)
    {
        return error("Cannot make directory", (int)file_errors::make_dir);
    }

    out << "GUI_DIR " << dir_name << endl;

    dir_manager.cd(QString::fromStdString(dir + "/" + dir_name));

    ofstream gui_writer;
    string gui_file = dir_name + "/GUI.data";

    gui_writer.open(gui_file);
    if (!gui_writer.is_open())
    {
        return error("Cannot open file", (int)file_errors::open);
    }

    graph *topology = storage->topology;

    std::map<object_id, it *> &objects = storage->items;
    std::map<link_id, QGraphicsLineItem *> &links = storage->links;

    for (auto &object : objects)
    {
        object_id obj = object.first;

        gui_writer << "MAKE_OBJECT ";

        gui_writer << get_name(topology->get_type(obj)) << " ";

        vertex *v = topology->get_object(obj);

        gui_writer << object.second->scenePos().x() << " " << object.second->scenePos().y() << " ";

        string data_file = dir_name + "/" + v->get_data()->get_name() + "_" + to_string(res_num) + ".data";

        gui_writer << data_file << endl;

        err = write_object_data(data_file, v);

        if (!err.is_ok())
            break;
    }

    if (err.is_ok())
        for (auto &l : links)
        {
            std::pair<object_id, object_id> connected_objs = topology->get_connected_object(l.first);

            gui_writer << "MAKE_LINK ";

            gui_writer << topology->get_object_name(connected_objs.first) << " " << topology->get_object_name(connected_objs.second) << endl;
        }

    gui_writer.close();

    return err;
}

error writer::write_results()
{
    return error(OK);
}

error writer::write_object_data(string data_file, vertex *v)
{
    ofstream data_writer;

    data_writer.open(data_file);

    if (!data_writer.is_open())
    {
        return error("Cannot create data file", (int)file_errors::open);
    }

    v->get_data()->write_data(data_writer);

    return error(OK);
}