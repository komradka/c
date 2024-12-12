#include "writer.hpp"
#include "../gui/graph_area.hpp"
#include "../manager_gui/settings_dialog.hpp"

error writer::write_settings()
{
    ofstream settings_writer;
    settings_writer.open(dir + "/settings.data");
    if (!settings_writer.is_open())
    {
        return error("Cannot open file", (int)file_errors::open);
    }
    out << "SETTINGS " << dir + "/settings.data" << endl;
    settings->write_settings(settings_writer);

    settings_writer.close();
    return error(OK);
}

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

    graph *topology = nullptr;// storage->topology;

    std::map<object_id, it *> &objects = storage->items;
    std::map<link_id, link_item *> &links = storage->links;

    for (auto &object : objects)
    {
        object_id obj = object.first;

        gui_writer << "MAKE_OBJECT ";

        gui_writer << get_name(topology->get_type(obj)) << " ";

        vertex *v = topology->get_object(obj);

        gui_writer << object.second->scenePos().x() << " " << object.second->scenePos().y() << " ";

        string obj_name = v->get_data()->get_name();
        std::replace(obj_name.begin(), obj_name.end(), ' ', '_');

        string data_file = dir_name + "/" + obj_name + "_" + to_string(res_num) + ".data";

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

            string obj_name_first = topology->get_object_name(connected_objs.first);
            string obj_name_second = topology->get_object_name(connected_objs.second);
            std::replace(obj_name_first.begin(), obj_name_first.end(), ' ', '_');
            std::replace(obj_name_second.begin(), obj_name_second.end(), ' ', '_');

            gui_writer << obj_name_first << " " << obj_name_second << endl;
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
