#include "initial_approximation/initial_approximation_builder.hpp"
#include "nd_results.hpp"
#include "main_equations.hpp"
#include "matrix_builder/matrix_builder.hpp"

#include <QtWidgets/QtWidgets>

#pragma once

class nd_solver : public QObject
{
    Q_OBJECT

private:
    reporter *rep;
    graph *network_topology;
    settings_dialog *settings;
    nd_solution *solution;
    initial_approximation_builder *initial_approximation;
    nd_results *results;
    main_equations *equations;
    matrix_builder *jacobian_builder;

public:
    error calculation_ret;

private:
    unsigned int dim;

public:
    nd_solver(QObject *parent, reporter *_rep, graph *nd_data, settings_dialog *_settings)
    {
        QObject::connect(this, SIGNAL(calculation_end()), parent, SLOT(calculation_end()));
        rep = _rep;
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

    error prepare_network_and_try_start_calculation();
    std::map<object_id, phys_q> &get_object_results();

private:
    error construct_main_equation();

    error write_results();
    error write_results_on_links();
    error write_results_on_object();

    void print_equations();

signals:
    void calculation_end();
};