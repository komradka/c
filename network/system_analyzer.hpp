#include <sys/sysinfo.h>
#include <sys/resource.h>

#pragma once

class system_analyzer
{
public:
    static int get_thread_count()
    {
        return get_nprocs();
    }
};
