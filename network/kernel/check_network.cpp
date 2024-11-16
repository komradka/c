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

    return error(OK);
}
