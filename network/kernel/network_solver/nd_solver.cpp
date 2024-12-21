#include "nd_solver.hpp"

#include "equations/nd_source_pressure_equation.hpp"
#include "equations/nd_source_rate_equation.hpp"
#include "equations/nd_pipe_pressure_equation.hpp"
#include "equations/nd_equality_pressure_equation.hpp"
#include "equations/nd_equality_rate_equation.hpp"
#include "equations/nd_joint_rate_equation.hpp"

error nd_solver::run (const thread_info &thr_info)
{
    bool visualize_initial_approximation = settings->get_param<solver_settings::visualize_initial_approximation>();

    //////////////////////////
    //      0  Prepare      //
    //////////////////////////

    if (thr_info.is_main_thread ())
      solution = new nd_solution(m_rep, network_topology, settings);
    thr_info.barrier ();

    /////////////////////////////////////
    //      1 Initial Approximation    //
    /////////////////////////////////////

    if (thr_info.is_main_thread ())
      initial_approximation = new initial_approximation_builder(m_rep, network_topology, settings, solution);
    thr_info.barrier ();

    initial_approximation->construct_initial_approximation(thr_info);

    if (!visualize_initial_approximation)
    {
        //////////////////////////////////////
        //      1 Construct equations       //
        //////////////////////////////////////

        error ret = construct_main_equation(thr_info);

        if (!ret.is_ok())
          return ret;

        if (thr_info.is_main_thread ())
          equations->print_eq();

        // jacobian_builder = new matrix_builder(equations, solution);
    }

    /////////////////////////////////
    //      & Write results       //
    ////////////////////////////////
    error ret = write_results(thr_info);

    if (!ret.is_ok())
      return ret;

    return error(OK);
}

error nd_solver::construct_main_equation(const thread_info &thr_info)
{
    if (thr_info.is_main_thread ())
      {
        bool use_temp = settings->get_param<solver_settings::use_temperature_equation>();
        (void)use_temp;

        equations = new main_equations(solution);

        std::vector<object_id> active_objects = network_topology->get_active_objects();

        // cout << active_objects.size() << endl;

        for (auto obj : active_objects)
        {
            auto type = network_topology->get_type(obj);
            auto name = network_topology->get_object_name(obj);

            std::vector<link_id> in_links = network_topology->get_inlet_links(obj);
            std::vector<link_id> out_links = network_topology->get_outlet_links(obj);

            switch (type)
            {
            case network_objects::source:
            {
                if (in_links.size() != 0 || out_links.size() != 1)
                {
                    return error("Wrong link count on object " + name);
                }
                unsigned int out_link_num = solution->get_link_num(out_links[0]);
                source_data *data = network_topology->get_source_data(obj);

                if (data->get_param<network_object_param::pressure>().has_value())
                {
                    RETURN_IF_FAIL(equations->set_pressure_equation({out_link_num}, new nd_source_pressure_equation(out_link_num, data)));
                }
                if (data->get_param<network_object_param::wrat>().has_value())
                {
                    RETURN_IF_FAIL(equations->set_rate_equation({out_link_num}, new nd_source_rate_equation(out_link_num, data)));
                }
                break;
            }
            case network_objects::pipe:
            {
                if (in_links.size() != 1 || out_links.size() != 1)
                {
                    return error("Wrong link count on object " + name);
                }

                unsigned int out_link_num = solution->get_link_num(out_links[0]);
                unsigned int in_link_num = solution->get_link_num(in_links[0]);

                pipe_data *data = network_topology->get_pipe_data(obj);

                RETURN_IF_FAIL(equations->set_pressure_equation({in_link_num, out_link_num},
                                                                new nd_pipe_pressure_equation(in_link_num, out_link_num, data)));

                RETURN_IF_FAIL(equations->set_rate_equation({in_link_num, out_link_num}, new nd_equality_rate_equation(in_link_num, out_link_num, data)));

                break;
            }
            case network_objects::sink:
            {
                if (in_links.size() != 1 || out_links.size() != 0)
                {
                    return error("Wrong link count on object " + name);
                }

                unsigned int in_link_num = solution->get_link_num(in_links[0]);

                sink_data *data = network_topology->get_sink_data(obj);

                if (data->get_param<network_object_param::pressure>().has_value())
                {
                    RETURN_IF_FAIL(equations->set_pressure_equation({in_link_num}, new nd_source_pressure_equation(in_link_num, data)));
                }
                if (data->get_param<network_object_param::wrat>().has_value())
                {
                    RETURN_IF_FAIL(equations->set_rate_equation({in_link_num}, new nd_source_rate_equation(in_link_num, data)));
                }

                break;
            }
            case network_objects::joint:
            {
                std::vector<unsigned int> links;
                for (unsigned int i = 0; i < in_links.size(); i++)
                {
                    links.push_back(solution->get_link_num(in_links[i]));
                }
                for (unsigned int i = 0; i < out_links.size(); i++)
                {
                    links.push_back(solution->get_link_num(out_links[i]));
                }

                unsigned int first_link = links[0];

                for (unsigned int i = 1; i < links.size(); i++)
                {
                    RETURN_IF_FAIL(equations->set_pressure_equation({first_link, links[i]}, new nd_equality_pressure_equation(first_link, links[i], nullptr)));
                }

                RETURN_IF_FAIL(equations->set_rate_equation(links, new nd_joint_rate_equation(in_links.size(), out_links.size(), links)));

                break;
            }
            case network_objects::link:
            case network_objects::unknown:
            case network_objects::COUNT:
            {
                break;
            }
            }
        }
      }

    thr_info.barrier ();
    return error(OK);
}

void nd_solver::print_equations()
{
}

error nd_solver::write_results(const thread_info &thr_info)
{
    if (thr_info.is_main_thread ())
      results = new nd_results();
    thr_info.barrier ();

    RETURN_IF_FAIL(write_results_on_links(thr_info));

    RETURN_IF_FAIL(write_results_on_object(thr_info));

    return error(OK);
}

error nd_solver::write_results_on_links(const thread_info &thr_info)
{
    if (thr_info.is_main_thread ())
      {
        std::vector<link_id> links = network_topology->get_links();

        for (link_id link : links)
        {
            phys_q quantity;
            quantity.construct_res_for_link(solution->get_link_num(link), solution);

            results->link_results[link] = quantity;
        }
      }

    thr_info.barrier ();
    return error(OK);
}

error nd_solver::write_results_on_object(const thread_info &thr_info)
{
    if (thr_info.is_main_thread ())
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
            case network_objects::joint:
            {

                break;
            }
            case network_objects::link:
            case network_objects::unknown:
            case network_objects::COUNT:
                break;
            }

            results->object_results[obj] = quantity;
        }
      }

    return error(OK);
}

const std::map<object_id, phys_q> &nd_solver::get_object_results()
{
    return results->object_results;
}
