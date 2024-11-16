#include "../../topology.hpp"
#include "../../../reporter.hpp"
#include "../../../manager_gui/settings_dialog.hpp"
#include "../nd_solution.hpp"

class initial_approximation_builder
{
private:
    reporter *rep;
    graph *network_topology;
    settings_dialog *settings;
    nd_solution *solution;

public:
    initial_approximation_builder(reporter *_rep, graph *_network_topology, settings_dialog *_settings, nd_solution *_solution)
    {
        rep = _rep;
        network_topology = _network_topology;
        settings = _settings;
        solution = _solution;
    }

    error construct_initial_approximation()
    {
        bool is_simple = settings->get_param<solver_settings::initial_approximation_type>() == initial_approximation_type::Simple;

        bool is_temp_used = settings->get_param<solver_settings::use_temperature_equation>();

        Q_UNUSED(is_temp_used);

        RETURN_IF_FAIL(construct_pressure_initial_approximation(is_simple));

        if (is_temp_used)
            RETURN_IF_FAIL(construct_temp_initial_approximation(is_simple));

        RETURN_IF_FAIL(construct_rate_initial_approximation(is_simple));

        return error(OK);
    }

private:
    error construct_pressure_initial_approximation(bool is_simple_ia);
    error construct_temp_initial_approximation(bool is_simple_ia);
    error construct_rate_initial_approximation(bool is_simple_ia);
    error simple_pressure_initial_approximation();
    error simple_temp_initial_approximation();
    error simple_rate_initial_approximation();
};