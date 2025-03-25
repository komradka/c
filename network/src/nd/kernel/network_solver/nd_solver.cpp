#include "nd_solver.hpp"

#include "equations/nd_source_pressure_equation.hpp"
#include "equations/nd_source_rate_equation.hpp"
#include "equations/nd_pipe_pressure_equation.hpp"
#include "equations/nd_equality_pressure_equation.hpp"
#include "equations/nd_equality_rate_equation.hpp"
#include "equations/nd_joint_rate_equation.hpp"
#include "common/base/include/format.hpp"

#include "thread_shared_ptr.h"

enum solver_error
{
    NON = 0,
    GLOBAL_ERR = 1,
    TEMP_ERR = 2,
    COMP_ERR
};

error nd_solver::run (const thread_info &thr_info)
{
    bool visualize_initial_approximation = settings->get_param<solver_settings::visualize_initial_approximation>();

    if (thr_info.is_main_thread())
    {
        data = new nd_data(network_topology);
    }

    error calculation_error = error(OK);

    for (int comp_it = 0; comp_it < data->get_component_count(); comp_it++)
    {
        graph_component *component = data->get_component(comp_it);
        //////////////////////////
        //      0  Prepare      //
        //////////////////////////

        if (thr_info.is_main_thread())
        {
            print_calculation_information(comp_it);
            calculation_error = verify_component(component);
            // TODO modification

        }

        if (!calculation_error.is_ok())
        {
            continue;
        }

        if (thr_info.is_main_thread ())
            solution = new nd_solution(m_rep, network_topology, component, settings);
        thr_info.barrier ();

        /////////////////////////////////////
        //      1 Initial Approximation    //
        /////////////////////////////////////

        if (thr_info.is_main_thread ())
            initial_approximation = new initial_approximation_builder(m_rep, network_topology, component, settings, solution);
        thr_info.barrier ();

        initial_approximation->construct_initial_approximation(thr_info);

        if (!visualize_initial_approximation)
        {
            //////////////////////////////////////
            //      2 Construct equations       //
            //////////////////////////////////////

            error ret = construct_main_equation(component, thr_info);

            if (!ret.is_ok())
                return ret;

            if (thr_info.is_main_thread ())
                equations->print_eq();


            newton (thr_info);
        }

        /////////////////////////////////
        //      3 Write results        //
        /////////////////////////////////

        error ret = write_results(thr_info);

        if (!ret.is_ok())
            return ret;


        /////////////////////////////////
        //      4 After Calculation    //
        /////////////////////////////////
        if (thr_info.is_main_thread())
        {
            clear_after_calculation();
            m_rep->print(message_type::MESSAGE, RUN_SECTION, "Component %d Successfully Calculated", comp_it);
        }
    }

    return error(OK);
}

error nd_solver::construct_main_equation(graph_component *component, const thread_info &thr_info)
{
    if (thr_info.is_main_thread ())
      {
        bool use_temp = settings->get_param<solver_settings::use_temperature_equation>();
        (void)use_temp;

        equations = new main_equations(solution);

        std::set<object_id> active_objects = component->get_active_object();

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
                joint_data *data = network_topology->get_joint_data(obj);

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
                    RETURN_IF_FAIL(equations->set_pressure_equation({first_link, links[i]}, new nd_equality_pressure_equation(first_link, links[i], data)));
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

error nd_solver::newton (const thread_info &thr_info)
{
  thread_shared_ptr<double[]> A (thr_info);
  thread_shared_ptr<size_t[]> I (thr_info);

  build_msr_matrix (A.get (), I.get (), thr_info);

  return error (OK);
}

error nd_solver::build_msr_matrix (double *A, size_t *I, const thread_info &thr_info)
{
  if (thr_info.is_main_thread ())
    jacobian_builder = new matrix_builder (equations, solution);
  thr_info.barrier ();

  size_t rows_count = equations->get_count ();

  size_t matrix_size = 0;
  thread_shared_ptr<std::unique_ptr<jacobian_row>[]> rows (thr_info, rows_count);
  for (size_t row_num : uniform_range_begin_end (rows_count, thr_info))
    {
      rows[row_num].reset (jacobian_builder->make_row (row_num));
      matrix_size += rows[row_num]->size;
    }
  thr_info.allreduce_sum (matrix_size);

  if (thr_info.is_main_thread ())
    {
      A = new double [matrix_size];
      I = new size_t [matrix_size + rows_count];
    }
  thr_info.barrier ();

  if (thr_info.is_main_thread ())
    {
      size_t index = 0;
      size_t prev_size = 0;
      for (size_t row_num : range (0ul, rows_count))
        {
          std::unique_ptr<jacobian_row> &row = rows[row_num];
          for (const row_element &non_zero_value : row->row)
            {
              A[index] = non_zero_value.value;
              I[index + rows_count] = non_zero_value.pos;
              index++;
            }
          I[row_num] = row->size + prev_size;
          prev_size = I[row_num];
        }
    }
  thr_info.barrier ();

  if (thr_info.is_main_thread ())
    {
      FILE *fp = fopen ("msr_debug_log.txt", "w");
      fprintf (fp, "size = %lu\n", rows_count);
      fprintf (fp, "A:\n");
      for (size_t i = 0; i < matrix_size; i++)
        fprintf (fp, "%lf ", A[i]);
      fprintf (fp, "\nI:\n");
      for (size_t i = 0; i < matrix_size + rows_count; i++)
        fprintf (fp, "%lu ", I[i]);
      fprintf (fp, "\n");
      fclose (fp);
    }

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

void nd_solver::clear_after_calculation()
{
    delete solution;
    delete initial_approximation;
    delete results;
    delete equations;
    delete jacobian_builder;
}

error nd_solver::verify_component(graph_component *comp)
{
    if (settings->get_param<solver_settings::use_temperature_equation> ())
    {
        return error ("Temperature option not supported", solver_error::COMP_ERR);
    }

    std::vector<object_id> boundary_objects = comp->get_active_boundary_object();

    unsigned boundary_condition_count = 0;
    for (object_id obj : boundary_objects)
    {
        boundary_condition_count += network_topology->get_boundary_condition_count_on_object(obj);
    }

    if (boundary_objects.size() != boundary_condition_count)
    {
        std::string err_msg = fmt::formating("Cannot calculate component. Boundary object count must be equal to Boundary condition count.\n"
                                             "Boundary object count %u\n"
                                             "Boundary condition count %u", boundary_objects.size(), boundary_condition_count);
        return error(err_msg); 
    }

    if (comp->get_active_object_by_type(network_objects::sink).size() < 1 ||
        comp->get_active_object_by_type(network_objects::source).size() < 1)
    {
        return error("Cannot calculate component without any source or sink");
    }

    return error(OK);
}

void nd_solver::print_calculation_information(int comp_it)
{
    std::string msg = "\n";
    msg += "==========================CALCULATION===============================\n";
    msg += fmt::formating("Start of calculation %d component\n", comp_it);

    graph_component *comp = data->get_component(comp_it);
    msg += fmt::formating("Object Count - %d\n", comp->get_object_count());
    msg += fmt::formating("Active Object Count - %d\n", comp->get_active_object_count());

    m_rep->print(message_type::MESSAGE, RUN_SECTION, msg.c_str());
}
