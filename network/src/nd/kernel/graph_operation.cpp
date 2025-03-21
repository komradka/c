#include "graph_operation.hpp"

nd_data::nd_data(graph *topology)
{
    this->topology = topology;
    create_components();

    for (graph_component *comp : connected_componets)
    {
        comp->update_active_obejct();
    }
}

nd_data::~nd_data()
{
    for (graph_component *comp : connected_componets)
    {
        delete comp;
    }
}

void nd_data::dfs(object_id v, graph_component *component, std::set<object_id> &visited)
{
    visited.insert(v);
    component->add_object(v);


    std::vector<object_id> neighbors = topology->get_neighbors(v);

    for (object_id n : neighbors)
    {
        if (!visited.count(n))
        {
            component->add_link(topology->get_link_between_object(v, n).value ());
            dfs(n, component, visited);
        }
    }
}

void nd_data::create_components()
{
    std::set<object_id> visited;

    std::vector<object_id> objects = topology->get_objects();

    for (object_id v : objects)
    {
        if (!visited.count(v))
        {
            graph_component *component = new graph_component(topology);
            dfs(v, component, visited);
            connected_componets.push_back(component);
            component_count++;
        }
    }
}

graph_component *nd_data::get_component(int index)
{
    if (index >= component_count)
    {
        std::abort;
    }
    return connected_componets[index];
}

int nd_data::get_component_count()
{
    return component_count;
}

graph_component::graph_component(graph *topology)
{
    this->topology = topology;
}

void graph_component::add_object(object_id v)
{
    connected_componet.insert(v);
    component_object_count++;
}

void graph_component::add_link(link_id l)
{
    component_links.insert(l);
    links_count++;
}

bool graph_component::contains(object_id v)
{
    return (!!connected_componet.count(v));
}

int graph_component::get_object_count()
{
    return component_object_count;
}

int graph_component::get_links_count()
{
    return links_count;
}

std::set<object_id> graph_component::get_objects()
{
    return connected_componet;
}

void graph_component::update_active_obejct()
{
    active_object.clear();

    for (object_id obj : connected_componet)
    {
        if (topology->is_active(obj))
        {
            active_object.insert(obj);
        }
    }
}

std::set<object_id> graph_component::get_active_object()
{
    return active_object;
}

std::set<link_id> graph_component::get_links ()
{
    return component_links;
}

std::vector<link_id> graph_component::get_links_v ()
{
    return std::vector<link_id>(component_links.begin(), component_links.end());
}

int graph_component::get_active_object_count()
{
    return active_object.size();
}

std::vector<object_id> graph_component::get_active_boundary_object()
{
    std::vector<object_id> res;
    for (object_id obj : connected_componet)
    {
        if (topology->is_boundary_object(obj))
            res.push_back(obj);
    }

    return res;
}

std::vector<object_id> graph_component::get_active_object_by_type(network_objects type)
{
    std::vector<object_id> res;

    for (object_id obj : active_object)
    {
        if (topology->get_type(obj) == type)
        {
            res.push_back(obj);
        }
    }

    return res;
}
