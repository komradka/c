#include "nd/iface/nd_manager.h"
#include "nd/iface/sim_manager.hpp"
#include "error.hpp"

#include <string>

#pragma once

enum class project_type
{
    network,
    simulator,

    COUNT
};

class project_manager
{
private:
    nd_manager *nd_project = nullptr;
    sim_manager *sim_project = nullptr;
    std::vector<pvt_manager *> pvt_projects;
    bool is_project_exist = false;

    std::string project_directory;

private:
    Slots m_slots;

public:
    error create_project(project_type type, int thread_num, std::string project_directory);
    error load_project(project_type type, int thread_num, std::string path);
    ~project_manager()
    {
        if (nd_project)
            delete nd_project;
        if (sim_project)
            delete sim_project;

        for (pvt_manager *pvt : pvt_projects)
            delete pvt;
    }

private:
    void make_nd_connection();

private:
    error create_nd_project(int thread_num);
    error create_sim_project(int thread_num);
    error create_pvt_project(project_type_t source, std::string name);

    error load_nd_project(int thread_num);
    error load_sim_project(int thread_num);
    error load_pvt_project(project_type_t source, std::string name, std::string path);
};
