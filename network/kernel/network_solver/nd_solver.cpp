#include "nd_solver.hpp"

error nd_solver::prepare_network_and_try_start_calculation()
{
    bool visualize_initial_approximation = settings->get_param<solver_settings::visualize_initial_approximation>();

    //////////////////////////
    //      0  Prepare      //
    //////////////////////////

    error ret = network_topology->verify_network_before_calculation();

    if (!ret.is_ok())
    {
        calculation_ret = ret;
        emit calculation_end();
        return ret;
    }

    solution = new nd_solution(rep, network_topology, settings);

    /////////////////////////////////////
    //      1 Initial Approximation    //
    /////////////////////////////////////

    initial_approximation = new initial_approximation_builder(rep, network_topology, settings, solution);

    initial_approximation->construct_initial_approximation();

    if (!visualize_initial_approximation)
    {
        //////////////////////////////////////
        //      1 Сonstruct equations       //
        //////////////////////////////////////

        construct_main_equation();
    }

    /////////////////////////////////
    //      & Write results       //
    ////////////////////////////////
    ret = write_results();

    if (!ret.is_ok())
    {
        calculation_ret = ret;
        emit calculation_end();
        return ret;
    }

    emit calculation_end();
    return error(OK);
}

error nd_solver::construct_main_equation()
{
    return error(OK);
}

error nd_solver::write_results()
{
    results = new nd_results();

    RETURN_IF_FAIL(write_results_on_links());

    RETURN_IF_FAIL(write_results_on_object());

    return error(OK);
}

error nd_solver::write_results_on_links()
{
    std::vector<link_id> links = network_topology->get_links();

    for (link_id link : links)
    {
        phys_q quantity;
        quantity.construct_res_for_link(solution->get_link_num(link), solution);

        results->link_results[link] = quantity;
    }

    return error(OK);
}

error nd_solver::write_results_on_object()
{

    std::vector<object_id> active_object = network_topology->get_active_objects();

    for (object_id obj : active_object)
    {
        auto type = network_topology->get_type(obj);

        std::vector<link_id> &inlet_links = network_topology->get_inlet_links(obj);
        std::vector<link_id> &outlet_links = network_topology->get_outlet_links(obj);

        phys_q quantity;

        switch (type)
        {
        case network_objects::source:
        {
            unsigned int link = solution->get_link_num(outlet_links[0]);

            quantity.construct_res_for_object(link, solution);
            break;
        }
        case network_objects::sink:
        {
            unsigned int link = solution->get_link_num(inlet_links[0]);
            quantity.construct_res_for_object(link, solution);
            break;
        }
        case network_objects::pipe:
        {
            unsigned int link = solution->get_link_num(inlet_links[0]);
            quantity.construct_res_for_object(link, solution);
            break;
        }
        case network_objects::link:
        case network_objects::unknown:
        case network_objects::COUNT:
            break;
        }

        results->object_results[obj] = quantity;
    }

    return error(OK);
}

std::map<object_id, phys_q> &nd_solver::get_object_results()
{
    return results->object_results;
}