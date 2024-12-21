#include "initial_approximation/initial_approximation_builder.hpp"
#include "nd_results.hpp"
#include "main_equations.hpp"
#include "matrix_builder/matrix_builder.hpp"

#include "nd_con/async_reporter.h"
#include "parallel/thread_info.h"

#pragma once

class nd_solver
{
private:
    async_reporter_t *m_rep;
    graph *network_topology;
    settings_dialog *settings;
    nd_solution *solution;
    initial_approximation_builder *initial_approximation;
    nd_results *results;
    main_equations *equations;
    matrix_builder *jacobian_builder;

private:
    unsigned int dim;

public:
    nd_solver(async_reporter_t *rep, graph *nd_data, settings_dialog *_settings)
    {
        m_rep = rep;
        network_topology = nd_data;
        settings = _settings;
    }

    ~nd_solver()
    {
        delete solution;
        delete initial_approximation;
        delete results;
        delete equations;
        delete jacobian_builder;
    }

    error run (const thread_info &thr_info);
    const std::map<object_id, phys_q> &get_object_results();

private:
    error construct_main_equation(const thread_info &thr_info);

    error write_results(const thread_info &thr_info);
    error write_results_on_links(const thread_info &thr_info);
    error write_results_on_object(const thread_info &thr_info);

    void print_equations();
};
