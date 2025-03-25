#pragma once

#include "nd/iface/topology.hpp"
#include "nd/kernel/graph_operation.hpp"
#include "nd/kernel/network_solver/nd_solution.hpp"

class nd_modification
{
private:
    graph *topology;
    graph_component *component;

public:
    nd_modification (graph *_topology, graph_component *_component);
    error modify();
    error restore_reslut(nd_solution *res);

};
