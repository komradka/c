#include "../../topology.hpp"
#include "nd_con/async_reporter.h"
#include "../../../manager_gui/settings_dialog.hpp"
#include "../nd_solution.hpp"
#include "parallel/thread_info.h"

class initial_approximation_builder
{
private:
    async_reporter_t *m_rep;
    graph *network_topology;
    settings_dialog *settings;
    nd_solution *solution;

public:
    initial_approximation_builder(async_reporter_t *rep, graph *_network_topology, settings_dialog *_settings, nd_solution *_solution)
    {
        m_rep = rep;
        network_topology = _network_topology;
        settings = _settings;
        solution = _solution;
    }

    error construct_initial_approximation(const thread_info &thr_info)
    {
        bool is_simple = settings->get_param<solver_settings::initial_approximation_type>() == initial_approximation_type::Simple;

        bool is_temp_used = settings->get_param<solver_settings::use_temperature_equation>();

        Q_UNUSED(is_temp_used);

        RETURN_IF_FAIL(construct_pressure_initial_approximation(is_simple, thr_info));

        if (is_temp_used)
            RETURN_IF_FAIL(construct_temp_initial_approximation(is_simple, thr_info));

        RETURN_IF_FAIL(construct_rate_initial_approximation(is_simple, thr_info));

        return error(OK);
    }

private:
    error construct_pressure_initial_approximation(bool is_simple_ia, const thread_info &thr_info);
    error construct_temp_initial_approximation(bool is_simple_ia, const thread_info &thr_info);
    error construct_rate_initial_approximation(bool is_simple_ia, const thread_info &thr_info);
    error simple_pressure_initial_approximation(const thread_info &thr_info);
    error simple_temp_initial_approximation(const thread_info &thr_info);
    error simple_rate_initial_approximation(const thread_info &thr_info);
};
