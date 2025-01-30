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

    m_slots.connect_to(nd_project->project_closed, [&]()
                       { is_project_exist = false;
                         delete nd_project;
                         nd_project = nullptr; });

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

    m_slots.connect_to(nd_project->project_closed, [&]()
                       { is_project_exist = false; });

    return error(OK);
}

error project_manager::load_sim_project(int thread_num)
{
    (void)thread_num;
    return error("Simulator in progress!");
}