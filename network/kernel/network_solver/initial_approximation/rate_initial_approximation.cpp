#include "initial_approximation_builder.hpp"

error initial_approximation_builder::construct_rate_initial_approximation(bool is_simple_ia)
{
    if (is_simple_ia)
        return simple_rate_initial_approximation();

    return error(OK);
}

error initial_approximation_builder::simple_rate_initial_approximation()
{
    double sum_rate = 0;
    int rate_objects = 0;

    std::vector<object_id> boundary_objects = network_topology->get_boundary_objects();

    auto calc_rate = [&](object_data *data)
    {
        auto opt_r = data->get_param<network_object_param::wrat>();

        sum_rate += opt_r.has_value() ? opt_r.value() : 0.;

        rate_objects += opt_r.has_value() ? 1 : 0;
    };

    for (auto obj : boundary_objects)
    {
        auto type = network_topology->get_type(obj);

        switch (type)
        {
        case network_objects::sink:
        {
            calc_rate(network_topology->get_sink_data(obj));
            break;
        }
        case network_objects::source:
        {
            calc_rate(network_topology->get_source_data(obj));
            break;
        }
        case network_objects::link:
        case network_objects::pipe:
        case network_objects::unknown:
        case network_objects::COUNT:
            break;
        }
    }

    if (rate_objects == 0)
        return error("No rate boundary condition");

    std::vector<link_id> links = network_topology->get_links();
    for (auto l : links)
    {
        unsigned int link_num = solution->get_link_num(l);

        solution->set_wrat(link_num, sum_rate);
    }

    rep->print_debug_message("Simple Rate initial approximation successfully constructed");
    return error(OK);
}