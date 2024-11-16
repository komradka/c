#include "initial_approximation_builder.hpp"

/// @brief mean of all pressure
/// @return
error initial_approximation_builder::simple_pressure_initial_approximation()
{
    double sum_pressure = 0;
    int pressure_count = 0;

    std::vector<object_id> boundary_objects = network_topology->get_boundary_objects();

    auto calc_pressure = [&](object_data *data)
    {
        auto opt_p = data->get_param<network_object_param::pressure>();

        sum_pressure += opt_p.has_value() ? opt_p.value() : 0.;
        pressure_count += opt_p.has_value() ? 1 : 0;
    };

    for (auto obj : boundary_objects)
    {
        auto type = network_topology->get_type(obj);

        switch (type)
        {
        case network_objects::sink:
        {
            calc_pressure(network_topology->get_sink_data(obj));
            break;
        }
        case network_objects::source:
        {
            calc_pressure(network_topology->get_source_data(obj));
            break;
        }
        case network_objects::link:
        case network_objects::pipe:
        case network_objects::unknown:
        case network_objects::COUNT:
            break;
        }
    }

    if (pressure_count == 0)
        return error("No pressure boundary condition");

    double average_pressure = sum_pressure / pressure_count;

    std::vector<link_id> links = network_topology->get_links();
    for (auto l : links)
    {
        unsigned int link_num = solution->get_link_num(l);

        solution->set_pressure(link_num, average_pressure);
    }

    rep->print_debug_message("Simple Pressure initial approximation successfully constructed");
    return error(OK);
}


error initial_approximation_builder::construct_pressure_initial_approximation(bool is_simple_ia)
{
    if (is_simple_ia)
        return simple_pressure_initial_approximation();

    return error(OK);
}

