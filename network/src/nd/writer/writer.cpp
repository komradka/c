#include "writer.hpp"
#include "nd/gui/graph_area.hpp"
#include "nd/gui/manager_gui/settings_dialog.hpp"

error check_and_create_dir(std::string path, std::string name)
{
    QDir dir_manager;

    if (dir_manager.exists(QString::fromStdString(path)))
    {
        return error(name + " Directory exists", (int)file_errors::exist);
    }
    if (dir_manager.mkdir(QString::fromStdString(path)) == false)
    {
        return error("Cannot make directory " + name, (int)file_errors::make_dir);
    }

    return error(OK);
}

error writer::prepare(bool &exist)
{
    if (project_out.is_open())
    {
        project_out.close();
    }

    project_dir = project_dir + "/" + project;

    main_project_file = project_dir + "/project.np";

    exist = std::filesystem::exists(main_project_file);

    if (res_num == 0)
    {
        check_and_create_dir(project_dir, "project directory");

        if (exist && res_num == 0)
            return error("File already exists", (int)file_errors::exist);
    }

    project_out.open(main_project_file, std::ios::app);

    if (!project_out.is_open())
    {
        return error("Cannot open project file", (int)file_errors::open);
    }

    result_dir = project_dir + "/" + res_name;

    check_and_create_dir (result_dir, "result directory");

    gui_dir = result_dir + "/" + paths.gui_dir_path;

    check_and_create_dir (gui_dir, "GUI directory");

    gui_out.open(result_dir + "/" + paths.gui_data_path);

    if (!gui_out.is_open())
    {
        return error("Cannot open gui file", (int)file_errors::open);
    }

    settings_out.open(result_dir + "/" + paths.settings_data_path);
    if (!settings_out.is_open())
    {
        return error("Cannot open gui file", (int)file_errors::open);
    }

    return error(OK);
}

error writer::write_settings()
{
    project_out << kwords.settings_path << " " << (res_name + "/" + paths.settings_data_path) << endl;
    settings->write_settings(settings_out);

    settings_out.close();
    return error(OK);
}

error writer::write_gui()
{
    error err(OK);

    std::string gui_dir = result_dir + "/" + paths.gui_dir_path;
    std::string gui_dir_rel = res_name + "/" + paths.gui_dir_path;

    project_out << kwords.gui_dir_path << " " << gui_dir_rel << endl;

    std::map<object_id, it *> &objects = storage->items;
    std::map<link_id, link_item *> &links = storage->links;

    for (auto &object : objects)
    {
        object_id obj = object.first;

        gui_out << kwords.make_object << " ";

        gui_out << get_name(topology->get_type(obj)) << " ";

        vertex *v = topology->get_object(obj);

        gui_out << object.second->scenePos().x() << " " << object.second->scenePos().y() << " ";

        string obj_name = v->get_data()->get_name();
        std::replace(obj_name.begin(), obj_name.end(), ' ', '_');

        string data_file = gui_dir + "/" + get_datafile_path (obj_name);

        gui_out << (gui_dir_rel + "/" + get_datafile_path (obj_name)) << endl;

        err = write_object_data(data_file, v);

        if (!err.is_ok())
            break;
    }

    if (err.is_ok())
        for (auto &l : links)
        {
            std::pair<object_id, object_id> connected_objs = topology->get_connected_object(l.first);

            gui_out << kwords.make_link << " ";

            string obj_name_first = topology->get_out_processed_name(connected_objs.first);
            string obj_name_second = topology->get_out_processed_name(connected_objs.second);

            gui_out << obj_name_first << " " << obj_name_second << endl;
        }

    gui_out.close();

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
