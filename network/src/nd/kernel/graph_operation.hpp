#include <vector>
#include <set>

#include "nd/iface/topology.hpp"

#pragma once

class graph_component
{
private:
    std::set<object_id> connected_componet;
    std::set<object_id> active_object;
    int component_object_count = 0;

    std::set<link_id> component_links;
    int links_count = 0;

    std::set<object_id> component_active_object;

    graph *topology = nullptr; // not owner

public:
    graph_component(graph *topology);

    void add_object(object_id);
    void add_link(link_id);
    bool contains(object_id);

    void update_active_obejct();
    std::set<object_id> get_active_object();
    std::set<object_id> get_objects();
    std::set<link_id> get_links ();
    std::vector<link_id> get_links_v ();
    int get_object_count();
    int get_links_count();
    int get_active_object_count();

    std::vector<object_id> get_active_boundary_object();
    std::vector<object_id> get_active_object_by_type(network_objects type);

    graph *get_topology();
};

class nd_data
{
private:
    std::vector<graph_component *> connected_componets;
    int component_count = 0;
    graph *topology = nullptr; // not owner

public:
    nd_data(graph *topology);
    ~nd_data();

    graph_component *get_component(int index);
    int get_component_count();

private:
    void dfs(object_id v, graph_component *component, std::set<object_id> &visited);
    void create_components();
};

//void
