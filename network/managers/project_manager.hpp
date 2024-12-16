#include "nd_manager.h"
#include "sim_manager.hpp"
#include "../error.hpp"
#include "../utility/signal_slots/slot.hpp"

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
    bool is_project_exist = false;

    std::string project_directory;

private:
    Slots m_slots;

public:
    error create_project(project_type type, int thread_num, std::string project_directory);
    ~project_manager()
    {
        if (nd_project)
            delete nd_project;
        if (sim_project)
            delete sim_project;
    }

private:
    error create_nd_project(int thread_num);
    error create_sim_project(int thread_num);
};