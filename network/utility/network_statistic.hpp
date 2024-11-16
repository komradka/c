#include <iostream>
#include <string>
#include <vector>

#pragma once

class network_statistic
{
private:
    unsigned int error_count = 0;
    unsigned int warning_count = 0;

public:
    void add_error()
    {
        error_count++;
    }

    void add_warning()
    {
        warning_count++;
    }

    void clear()
    {
        error_count = 0;
        warning_count = 0;
    }
};
