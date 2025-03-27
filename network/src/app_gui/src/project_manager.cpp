#include "project_manager.hpp"

#include <iostream>

error project_manager::create_project(project_type type, int thread_num, std::string project_directory)
{
    if (is_project_exist)
    {
        return error("Project already exist.\nOnly single process mode.");
    }

    this->project_directory = project_directory + ".np";

    switch (type)
    {
    case project_type::network:
        return create_nd_project(thread_num);
    case project_type::simulator:
        return create_sim_project(thread_num);
    case project_type::COUNT:
        break;
    }

    return error(OK);
}

error project_manager::create_nd_project(int thread_num)
{
    nd_project = new nd_manager(project_directory);
    nd_project->create_kernel_threads(thread_num);
    is_project_exist = true;

    make_nd_connection();

    return error(OK);
}

error project_manager::create_sim_project(int thread_num)
{
    (void)thread_num;
    return error("Simulator in progress!");
}

error project_manager::load_project(project_type type, int thread_num, std::string path)
{
    if (is_project_exist)
    {
        return error("Project already exist.\nOnly single process mode.");
    }

    auto found = path.find_last_of("/\\");
    std::string dir = path.substr(0, found);
    this->project_directory = dir;

    switch (type)
    {
    case project_type::network:
        return load_nd_project(thread_num);
    case project_type::simulator:
        return load_sim_project(thread_num);
    case project_type::COUNT:
        break;
    }

    return error(OK);
}

error project_manager::load_nd_project(int thread_num)
{
    nd_project = new nd_manager(project_directory);
    nd_project->create_kernel_threads(thread_num);
    is_project_exist = true;

    nd_project->load_project_handler();

    make_nd_connection ();

    return error(OK);
}

error project_manager::load_sim_project(int thread_num)
{
    (void)thread_num;
    return error("Simulator in progress!");
}

error project_manager::create_pvt_project(project_type_t source, std::string name)
{
    pvt_manager *new_fluid = new pvt_manager(name);

    switch (source)
    {
    case project_type_t::network_designer:
    {
        nd_project->network_PVT.push_back(new_fluid);
    }
    case project_type_t::main:
    case project_type_t::pipe_designer:
    case project_type_t::pvt_designer:
    case project_type_t::simulator:
    case project_type_t::none:
    case project_type_t::COUNT:
        break;
    }

    pvt_projects.push_back(new_fluid);

    return error(OK);
}

error project_manager::load_pvt_project(project_type_t source, std::string name, std::string path)
{
    pvt_manager *new_fluid = new pvt_manager(name, path);

    switch (source)
    {
    case project_type_t::network_designer:
    {
        nd_project->network_PVT.push_back(new_fluid);
    }
    case project_type_t::main:
    case project_type_t::pipe_designer:
    case project_type_t::pvt_designer:
    case project_type_t::simulator:
    case project_type_t::none:
    case project_type_t::COUNT:
        break;
    }

    pvt_projects.push_back(new_fluid);

    return error(OK);
}

void project_manager::make_nd_connection()
{
    m_slots.connect_to(nd_project->project_closed, [&]()
                       { is_project_exist = false; });

    m_slots.connect_to(nd_project->create_fluid_signal, [&](std::string name)
                       { create_pvt_project(project_type_t::network_designer, name); });

    m_slots.connect_to(nd_project->export_fluid_signal, [&](std::string name, std::string path)
                       { load_pvt_project(project_type_t::network_designer, name, path); });
}
