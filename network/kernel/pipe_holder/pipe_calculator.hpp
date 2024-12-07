#include "../objects_data/pipe_data.hpp"
#include "../internal_constant.hpp"
#include "pipe_params.hpp"
#include "../fluid/fluid_props.hpp"
#include <vector>

#pragma once

class pipe_calculator
{
private:
    pipe_data *data;
    fluid_props *fluid;
    pipe_correlation corr = pipe_correlation::Moody;

    const int report_segments_count = 200;
    std::vector<pipe_param> report_segments;

    std::vector<pipe_param> segments;

public:
    pipe_calculator(pipe_data *data, fluid_widget *fw)
    {
        report_segments.resize(report_segments_count);
        this->data = data;
        fluid = new fluid_props(fw);
    }

    error compute_pressure_drop(double mass_rate, double t_out, double p_out, double &pressure_drop);
    error compute_pressure_and_temperature_drop(double mass_rate, double t_in, double p_out, double &pressure_drop, double &temp_drop);
    std::vector<pipe_param> &get_report_params() { return report_segments; }

private:                                       // initial params
    const double segment_initial_length = 5;   // [m]
    const double segment_min_length = 5;       // [m]
    const double segment_max_length = 100;     // [m]
    const double segment_error_max = 0.5;      // [bar]
    const double pipe_error_max = 1.;          // [bar]
    const double segment_dp_max = 20.;         // [bar]
    const double segment_error_min = 0.01;     // [bar]
    const double pressure_convergence = 0.001; // [bar]
    const double converge_error = 1e-6;
    const int segment_length_factor = 3;
    const int max_pressure_iter = 15;
    const int max_pressure_and_temperature_converge_iter = 50;

private:
    double compute_constant_pressure_drop(double length);
    double compute_friction_pressure_diff(double rho, double visc, double v, double length);
    double compute_re(double rho, double visc, double v);
    double compute_1_sqrt_f_old(double rho, double visc, double v);
    double compute_fc_by_Moody(double rho, double visc, double v);
    double compute_friction_coef(double rho, double visc, double v);
    error compute_segment_pressure_drop(double length, double mass_rate, double t_out, double p_out, double &dp);
    double compute_velocity(double mass_rate, double density, double diameter);
    error gradient_method(double mass_rate, double p_out, double t_out, double &pressure_drop);
    double compute_hydrostatic_pressure_drop(double rho, double seg_len);
    void init_segment_params(double p_out, double p_in, double t_out, double t_in, double l, double pressure_drop, double pressure_grad, double pressure_error);
    void resegment();
    error gradient_method_with_temp(double mass_rate, double p_out, double t_in, double &pressure_drop, double &temp_drop);
    error compute_temperature_drop(double t_in, double mass_rate, double &t_out);
    error compute_segment_temperature(double p_in, double p_out, double t_in, double mass_rate, double seg_len, double &t_out);
    error compute_inside_heat_transfer_coef(double length, double mass_rate, double pressure, double temperature, double &htc);
    error compute_layer_heat_transfer_coef(double layer_conductivity, double outer_layer_diam, double inner_layer_diam, double outer_pipe_diam, double &htc);
    error compute_overall_heat_transfer_coef(double length, double mass_rate, double pressure, double temperature, double &htc);
};