#include "pipe_calculator.hpp"
#include <math.h>

double pipe_calculator::compute_re(double rho, double visc, double v)
{
    double _rho = rho * 0.06243;
    double _v = v * 3.281;
    double _D = data->get_param<network_object_param::pipe_inner_diameter>().value() * 3.281;

    return 1488. * _rho * _v * _D / visc;
}

double pipe_calculator::compute_1_sqrt_f_old(double rho, double visc, double v)
{
    double re = compute_re(rho, visc, v);

    double specific_roughness = data->get_param<network_object_param::pipe_roughness>().value() / data->get_param<network_object_param::pipe_inner_diameter>().value();

    double A = pow(specific_roughness, 1.1098) * 0.3539;
    double B = 5.8506 / pow(re, 0.8981);

    double C = 5.0452 * log(A + B) / re;
    double D = 0.2698 * specific_roughness;

    return -4. * log(D - C);
}

double pipe_calculator::compute_friction_pressure_diff(double rho, double visc, double v, double length)
{
    double f = compute_friction_coef(rho, visc, v);
    double friction_drop = 0.5 * f * v * v * rho * length / constant::BAR_TO_PA / data->get_param<network_object_param::pipe_inner_diameter>().value();

    return friction_drop;
}

double pipe_calculator::compute_constant_pressure_drop(double length)
{
    return length * data->get_param<network_object_param::pipe_pressure_drop_coef>().value();
}

error pipe_calculator::compute_pressure_drop(double mass_rate, double t_out, double p_out, double &pressure_drop)
{
    pipe_type type = data->get_param<network_object_param::pipe_type>();

    if (type == pipe_type::constant_coef)
    {
        double res_pressure_drop = data->get_param<network_object_param::pipe_len>().value() * data->get_param<network_object_param::pipe_pressure_drop_coef>().value();

        double seg_len = data->get_param<network_object_param::pipe_len>().value() / report_segments_count;

        report_segments[report_segments_count - 1].get_param(pipe_param_type::pressure_end) = p_out;

        for (int i = report_segments_count - 1; i >= 0; i--)
        {
            double seg_pressure_drop = seg_len * data->get_param<network_object_param::pipe_pressure_drop_coef>().value();
            report_segments[i].get_param(pipe_param_type::pressure_begin) =
                report_segments[i].get_param(pipe_param_type::pressure_end) + seg_pressure_drop;

            if (i != 0)
                report_segments[i - 1].get_param(pipe_param_type::pressure_end) = report_segments[i].get_param(pipe_param_type::pressure_begin);
        }

        pressure_drop = res_pressure_drop;
        return error(OK);
    }
    else
    {
        return gradient_method(mass_rate, p_out, t_out, pressure_drop);
    }
}

error pipe_calculator::gradient_method(double mass_rate, double p_out, double t_out, double &p_in)
{
    segments.clear();

    bool need_increase_l = false;
    bool need_decrease_l = false;
    // bool was_increase = false;

    double l = data->get_param<network_object_param::pipe_len>().value();

    double l_ost = l;
    double curr_seg_l;
    double prev_seg_l = segment_initial_length;

    l_ost -= curr_seg_l;

    double dp;
    double prev_dp;

    double curr_p_end;
    double curr_p_begin;
    double prev_p_end = p_out;
    double prev_p_begin;

    double prev_p_grad;
    double curr_p_grad;

    double sum_dp = 0;
    double sum_err = 0;

    compute_segment_pressure_drop(prev_seg_l, mass_rate, t_out, prev_p_end, prev_dp);

    sum_dp += prev_dp;

    prev_p_begin = prev_p_end + prev_dp;
    prev_p_grad = prev_dp / prev_seg_l;

    init_segment_params(prev_p_end, prev_p_begin, t_out, t_out, prev_seg_l, prev_dp, prev_p_grad, 0);

    while (l_ost > 0)
    {
        if (need_increase_l)
        {
            curr_seg_l = prev_seg_l * segment_length_factor > segment_max_length ? segment_max_length : prev_seg_l * segment_length_factor;
            need_increase_l = false;
        }
        else if (need_decrease_l && fabs(curr_seg_l - segment_min_length) > 1e-4)
        {
            curr_seg_l = prev_seg_l / segment_length_factor < segment_min_length ? segment_min_length : prev_seg_l / segment_length_factor;
            need_decrease_l = false;
        }
        else
        {
            curr_seg_l = prev_seg_l;
        }

        if (curr_seg_l > l_ost)
        {
            curr_seg_l = l_ost;
        }
        curr_p_end = prev_p_begin;

        compute_segment_pressure_drop(curr_seg_l, mass_rate, t_out, curr_p_end, dp);
        sum_dp += dp;

        curr_p_begin = curr_p_end + dp;
        curr_p_grad = dp / curr_seg_l;

        double curr_error = fabs(curr_p_grad * curr_seg_l - prev_p_grad * curr_seg_l);
        sum_err += curr_error;

        if ((curr_error > segment_error_max || dp > segment_dp_max) && !need_decrease_l)
        {
            need_decrease_l = true;
        }
        else if (curr_error < segment_error_min && l_ost - curr_seg_l > 0 && !need_increase_l)
        {
            need_increase_l = true;
        }

        init_segment_params(curr_p_end, curr_p_begin, t_out, t_out, curr_seg_l, dp, curr_p_grad, curr_error);

        prev_p_end = curr_p_end;
        prev_p_begin = curr_p_begin;
        prev_p_grad = curr_p_grad;
        l_ost -= curr_seg_l;
        prev_seg_l = curr_seg_l;
    }

    if (fabs(sum_err) >= pipe_error_max)
    {
        return error("Calculation did not converge");
    }

    resegment();
    p_in = p_out + sum_dp;
    return error(OK);
}

error pipe_calculator::compute_segment_pressure_drop(double length, double mass_rate, double t_out, double p_out, double &dp)
{
    double rho = 0, visc = 0, v;
    RETURN_IF_FAIL(fluid->compute_density(p_out, t_out, rho));
    RETURN_IF_FAIL(fluid->compute_viscosity(p_out, t_out, visc));

    v = compute_velocity(mass_rate, rho, data->get_param<network_object_param::pipe_inner_diameter>().value());

    double friction_drop = compute_friction_pressure_diff(rho, visc, v, length);
    double hydrostatic_drop = compute_hydrostatic_pressure_drop(rho, length);

    dp = hydrostatic_drop + friction_drop;
    return error(OK);
}

double pipe_calculator::compute_velocity(double mass_rate, double density, double diameter)
{
    double area = M_PI_4 * diameter * diameter;
    double sec_to_day = 1. / 24. / 3600.;
    return mass_rate * sec_to_day / area / density;
}

double pipe_calculator::compute_friction_coef(double rho, double visc, double v)
{
    if (corr == pipe_correlation::old)
    {
        double f = compute_1_sqrt_f_old(rho, visc, v);
        return 1 / f / f;
    }
    if (corr == pipe_correlation::Moody)
    {
        return compute_fc_by_Moody(rho, visc, v);
    }
    return 0;
}

double pipe_calculator::compute_fc_by_Moody(double rho, double visc, double v)
{
    const double lam_re = 2000.;
    const double tur_re = 4000.;

    double re = compute_re(rho, visc, v);

    auto compute_f_laminar = [&]() -> double
    {
        return 64 / re;
    };

    auto compute_f_turbulent = [&]() -> double
    {
        double a = 2 / log(10);
        double c = re * log(10) / 5.02;
        double specific_roughness = data->get_param<network_object_param::pipe_roughness>().value() / data->get_param<network_object_param::pipe_inner_diameter>().value();
        double b = specific_roughness / 3.7;
        double s = b * c + log(c);
        double q = pow(s, s / (s + 1));
        double g = b * c + log(c / q);
        double z = log(q / g);
        double d = g / (g + 1) * z;
        double f = a * (log(c / q) + d);
        return 1 / f * f;
    };

    if (re < lam_re)
    {
        return compute_f_laminar();
    }
    else if (re > tur_re)
    {
        return compute_f_turbulent();
    }

    double f_lam = compute_f_laminar();
    double f_tur = compute_f_turbulent();

    return (re - lam_re) * (f_tur - f_lam) / (tur_re - lam_re) + f_lam;
}

double pipe_calculator::compute_hydrostatic_pressure_drop(double rho, double seg_len)
{
    double seg_height_diff = seg_len / data->get_param<network_object_param::pipe_len>().value() * data->get_param<network_object_param::pipe_height_diff>().value();
    return rho * constant::g_metric * seg_height_diff / constant::BAR_TO_PA;
}

void pipe_calculator::init_segment_params(double p_out, double p_in, double t_out, double t_in, double l, double pressure_drop, double pressure_grad, double pressure_error)
{
    pipe_param param;
    param.get_param(pipe_param_type::pressure_end) = p_out;
    param.get_param(pipe_param_type::pressure_begin) = p_in;
    param.get_param(pipe_param_type::temperature_end) = t_out;
    param.get_param(pipe_param_type::temperature_begin) = t_in;
    param.get_param(pipe_param_type::length) = l;
    param.get_param(pipe_param_type::pressure_drop) = pressure_drop;
    param.get_param(pipe_param_type::pressure_gradient) = pressure_grad;
    param.get_param(pipe_param_type::pressure_error) = pressure_error;

    segments.push_back(param);
}

void pipe_calculator::resegment()
{
    std::reverse(segments.begin(), segments.end());

    double rep_seg_len = data->get_param<network_object_param::pipe_len>().value() / report_segments_count;

    auto find_seg_for_rep_seg_begin = [&](double rep_seg_begin, double &seg_begin, double &seg_end, pipe_param &seg)
    {
        double prev_l = 0;
        for (auto s : segments)
        {
            prev_l += s.get_param(pipe_param_type::length);
            if (prev_l > rep_seg_begin)
            {
                seg = s;
                seg_end = prev_l;
                seg_begin = prev_l - s.get_param(pipe_param_type::length);
                break;
            }
        }
        seg_begin = -1;
        seg_end = -1;
    };

    double rep_seg_begin = 0.;
    double rep_seg_end = rep_seg_begin + rep_seg_len;
    for (unsigned int i = 0; i < report_segments.size(); i++)
    {
        double seg_begin;
        double seg_end;
        pipe_param seg;
        find_seg_for_rep_seg_begin(rep_seg_begin, seg_begin, seg_end, seg);

        using p = pipe_param_type;

        if (rep_seg_end <= seg_end)
        {
            double rel_len = rep_seg_len / seg.get_param(p::length);
            report_segments[i].get_param(p::length) = rep_seg_len;
            report_segments[i].get_param(p::pressure_drop) = seg.get_param(p::pressure_drop) * rel_len;
            report_segments[i].get_param(p::pressure_error) = seg.get_param(p::pressure_error) * rel_len;
            report_segments[i].get_param(p::pressure_gradient) = seg.get_param(p::pressure_gradient);

            double rel_begin_len = (rep_seg_begin - seg_begin) / seg.get_param(p::length);
            report_segments[i].get_param(p::pressure_begin) = seg.get_param(p::pressure_begin) - seg.get_param(p::pressure_drop) * rel_begin_len;
            double rel_end_len = (rep_seg_end - seg_begin) / seg.get_param(p::length);
            report_segments[i].get_param(p::pressure_end) = seg.get_param(p::pressure_begin) - seg.get_param(p::pressure_drop) * rel_end_len;
        }
        else
        {
            std::cout << rep_seg_end << std::endl;
            std::cout << seg_end << std::endl;
        }

        rep_seg_begin = rep_seg_end;
        rep_seg_end = rep_seg_begin + rep_seg_len;
    }
}

error pipe_calculator::compute_pressure_and_temperature_drop(double mass_rate, double t_in, double p_out, double &pressure_drop, double &temp_drop)
{
    gradient_method_with_temp(mass_rate, p_out, t_in, pressure_drop, temp_drop);

    return error(OK);
}

error pipe_calculator::gradient_method_with_temp(double mass_rate, double p_out, double t_in, double &pressure_drop, double &temp_drop)
{
    int iter = 0;
    double pressure_diff = 1.;
    double temperature_diff = 1.;

    double prev_t_out = 0., prev_p_in = 0.;
    double t_out = 322.6, p_in = 0.;

    while (iter <= max_pressure_and_temperature_converge_iter && pressure_diff > converge_error && temperature_diff > converge_error)
    {
        RETURN_IF_FAIL(gradient_method(mass_rate, p_out, t_out, p_in));

        RETURN_IF_FAIL(compute_temperature_drop(t_in, mass_rate, t_out));

        temperature_diff = fabs(t_out - prev_t_out);
        pressure_diff = fabs(p_in - prev_p_in);

        prev_t_out = t_out;
        prev_p_in = p_in;

        iter++;
    }
    if (iter > max_pressure_and_temperature_converge_iter)
    {
        return error("Did not converge");
    }

    pressure_drop = p_in - p_out;
    temp_drop = t_in - t_out;

    return error(OK);
}

error pipe_calculator::compute_temperature_drop(double t_in, double mass_rate, double &t_out)
{
    std::reverse(segments.begin(), segments.end());

    segments[0].get_param(pipe_param_type::temperature_begin) = t_in;

    for (unsigned int i = 0; i < segments.size(); i++)
    {
        double p_in = segments[i].get_param(pipe_param_type::pressure_begin);
        double p_out = segments[i].get_param(pipe_param_type::pressure_end);
        double t_in = segments[i].get_param(pipe_param_type::temperature_begin);
        double l = segments[i].get_param(pipe_param_type::length);
        double seg_t_out;
        RETURN_IF_FAIL(compute_segment_temperature(p_in, p_out, t_in, mass_rate, l, seg_t_out));

        segments[i].get_param(pipe_param_type::temperature_end) = seg_t_out;
        segments[i].get_param(pipe_param_type::temperature_drop) = t_in - seg_t_out;
        if (i != segments.size() - 1)
            segments[i + 1].get_param(pipe_param_type::temperature_begin) = seg_t_out;
    }

    t_out = segments[segments.size() - 1].get_param(pipe_param_type::temperature_end);

    return error(OK);
}

error pipe_calculator::compute_inside_heat_transfer_coef(double length, double mass_rate, double pressure, double temperature, double &htc)
{
    double rho = 0;
    RETURN_IF_FAIL(fluid->compute_density(pressure, temperature, rho));
    double visc = 0;
    RETURN_IF_FAIL(fluid->compute_viscosity(pressure, temperature, visc));
    double v = compute_velocity(mass_rate, rho, data->get_param<network_object_param::pipe_inner_diameter>().value());
    double re = compute_re(rho, visc, v);

    double pr = visc * fluid->get_water_heat_capacity() / fluid->get_water_conductivity();

    const double re_min = 2000;
    const double re_max = 6000;

    double nu = 0;
    double rel_length = length / data->get_param<network_object_param::pipe_inner_diameter>().value();

    auto compute_tur_nu = [&]() -> double
    {
        return 0.023 * pow(re, 0.8) * pow(pr, 0.33) * (1 + pow(1. / rel_length, 0.7));
    };

    auto compute_lam_nu = [&]() -> double
    {
        double nu1 = -25. * re * pr / rel_length * log(1. - 2.645 / pow(pr, 0.167) / sqrt(re * pr / rel_length));
        double nu3 = 1.86 * pow(re * pr / rel_length, 1 / 3);

        double nu2 = -(1. - (1 / rel_length - 10.) / 20.) * nu1 + (1. / rel_length) / 20. * nu3;

        if (rel_length < 10)
            return nu1;
        if (rel_length > 30)
            return nu3;
        return nu2;
    };

    if (re > re_max)
    {
        nu = compute_tur_nu();
    }
    else if (re < re_min)
    {
        nu = compute_lam_nu();
    }
    else
    {
        double nu1 = compute_lam_nu();
        double nu2 = compute_tur_nu();

        double e = (log(nu2) - log(nu1)) / (log(re_max) - log(re_min));
        nu = nu1 * pow(re / 2000., e);
    }

    htc = nu * fluid->get_water_conductivity() / data->get_param<network_object_param::pipe_inner_diameter>().value();

    return error(OK);
}

error pipe_calculator::compute_layer_heat_transfer_coef(double layer_conductivity, double outer_layer_diam, double inner_layer_diam,
                                                        double outer_pipe_diam, double &htc)
{
    double denom = outer_pipe_diam * log(outer_layer_diam / inner_layer_diam);

    htc = 2 * layer_conductivity / denom;
    return error(OK);
}

error pipe_calculator::compute_overall_heat_transfer_coef(double length, double mass_rate, double pressure, double temperature, double &htc)
{
    if (data->get_param<network_object_param::htc>())
    {
        htc = data->get_param<network_object_param::htc>().value();
        return error(OK);
    }

    double htc_inside;
    RETURN_IF_FAIL(compute_inside_heat_transfer_coef(length, mass_rate, pressure, temperature, htc_inside));

    double htc_layer;
    double cond = data->get_param<network_object_param::pipe_wall_cond>().value();
    double inner_layer_diam = data->get_param<network_object_param::pipe_inner_diameter>().value();
    double outer_layer_diam = inner_layer_diam + 2 * data->get_param<network_object_param::pipe_wall_thickness>().value();
    double outer_pipe_diam = outer_layer_diam;
    RETURN_IF_FAIL(compute_layer_heat_transfer_coef(cond, outer_layer_diam, inner_layer_diam, outer_pipe_diam, htc_layer));

    double htc1 = 1. / (1. / htc_inside + 1. / htc_layer);
    htc = htc1;

    return error(OK);
}

error pipe_calculator::compute_segment_temperature(double p_in, double p_out, double t_in, double mass_rate, double seg_len, double &t_out)
{
    double htc;
    compute_overall_heat_transfer_coef(seg_len, mass_rate, p_in, t_in, htc);

    double mr = mass_rate / 24 / 3600;

    double enthalpy_inlet = 0;
    fluid->compute_enthalpy(t_in, p_in, enthalpy_inlet);

    double ambient_temperature = data->get_param<network_object_param::ambient_temperature>().value() - constant::ABS_ZERO_IN_CELSIUS;
    double seg_height_diff = seg_len / data->get_param<network_object_param::pipe_len>().value() * data->get_param<network_object_param::pipe_height_diff>().value();

    double water_density;
    fluid->compute_density_sc(water_density);
    double specific_density = water_density / constant::water_density_sc;

    double potential_energy = constant::g_metric * seg_height_diff / constant::kilo;

    double outside_radius = data->get_param<network_object_param::pipe_inner_diameter>().value() / 2 + data->get_param<network_object_param::pipe_wall_thickness>().value();
    double area = 2 * M_PI * outside_radius * seg_len;

    double Q_a = htc * area / mr * ambient_temperature / constant::kilo;

    double numerator = enthalpy_inlet + Q_a - potential_energy - (8.786214e-5 / specific_density) * p_out + 255.37 * fluid->get_water_heat_capacity();
    double denominator = fluid->get_water_heat_capacity() + htc * area / mr / constant::kilo;

    t_out = numerator / denominator;

    return error(OK);
}
