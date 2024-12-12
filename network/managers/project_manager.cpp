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

    return error(OK);
}

error project_manager::create_sim_project(int thread_num)
{
    (void)thread_num;
    return error("Simulator in progress!");
}