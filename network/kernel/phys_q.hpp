#include "phys_q_type.hpp"
#include <map>

#ifndef PQ

#define PQ

class phys_q
{
    std::map<phys_q_type, double> data;

public:
    std::map<phys_q_type, double> &get_modifiable_q()
    {
        return data;
    }

};

#endif