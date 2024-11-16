#include "../phys_q.hpp"
#include <map>

using object_id = int;
using link_id = int;

class nd_results
{
public:
    std::map<link_id, phys_q> link_results;

    std::map<object_id, phys_q> object_results;

public:
    void copy(nd_results &rhs)
    {
        link_results = rhs.link_results;
        object_results = rhs.object_results;
    }
};