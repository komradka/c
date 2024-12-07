#include "../error.hpp"

#pragma once

class graph;

error check_objects(graph *network_topology);
error check_link_count(graph *network_topology);