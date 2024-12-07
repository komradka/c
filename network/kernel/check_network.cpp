#include "check_network.hpp"
#include "topology.hpp"

std::string make_validation_error(std::string message)
{
    return "Network Validation: " + message;
}

int get_boundary_conditions_count(std::vector<object_id> boundary_objects, graph *network_topology)
{
    int res = 0;
    for (object_id obj : boundary_objects)
    {
        res += network_topology->get_boundary_condition_count_on_object(obj);
    }
    return res;
}

error check_link_count(graph *network_topology)
{
    auto active_object = network_topology->get_active_objects();

    for (object_id obj : active_object)
    {
        network_objects type = network_topology->get_type(obj);
        std::string name = network_topology->get_object_name(obj);
        std::string error_message = "Object " + name + " has wrong link count";

        int inlet_links_count = network_topology->get_inlet_links(obj).size();
        int outlet_links_count = network_topology->get_outlet_links(obj).size();

        switch (type)
        {
        case network_objects::pipe:
            if (inlet_links_count != 1 || outlet_links_count != 1)
                return error(error_message);
            break;
        case network_objects::sink:
            if (inlet_links_count != 1 || outlet_links_count != 0)
                return error(error_message);
            break;
        case network_objects::source:
            if (inlet_links_count != 0 || outlet_links_count != 1)
                return error(error_message);
            break;
        case network_objects::joint:
            if (inlet_links_count == 0 || outlet_links_count == 0)
                return error(error_message);
            break;
        case network_objects::COUNT:
        case network_objects::link:
        case network_objects::unknown:
            break;
        }
    }

    return error(OK);
}

error check_objects(graph *network_topology)
{
    if (network_topology->get_active_objects_by_type(network_objects::sink).size() < 1 ||
        network_topology->get_active_objects_by_type(network_objects::source).size() < 1)
    {
        return error(make_validation_error("Cannot calculate network without any source or sink."));
    }

    std::vector<object_id> boundary_objects = network_topology->get_boundary_objects();

    int boundary_object_count = boundary_objects.size();
    int boundary_condition_count = get_boundary_conditions_count(boundary_objects, network_topology);

    if (boundary_object_count != boundary_condition_count)
    {
        return (make_validation_error("Cannot calculate network. Boundary object count must be equal to Boundary condition count.\nBoundary object count is -" + to_string(boundary_object_count) + "\nBoundary condition count is -" + to_string(boundary_condition_count)));
    }

    RETURN_IF_FAIL(check_link_count(network_topology));

    return error(OK);
}
