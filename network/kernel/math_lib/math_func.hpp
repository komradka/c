#include <iostream>
#include <math.h>
#include <functional>

#pragma once

namespace math_const
{
    const double default_eps = 1e-3;
    const double default_delta = 1e-8;
    const int max_it = 100;
}

enum class derivative_type
{
    linear,
    quadric,
};

class Math
{
public:
    /**
     * @brief calculate numeric derivative:
     *        if (type == linear => (f(x + delta) - f(x)) / delta )
     *        if type == quadric => (f(x + delta) - f(x - delta)) / (2 * delta)
     *
     * @param f
     * @param delta
     * @param type
     * @return double
     */
    static double calculate_numeric_derivative(std::function<double(double)> f,
                                               const double x, 
                                               const double delta = math_const::default_delta,
                                               const derivative_type type = derivative_type::linear);



private:
    static double calculate_numeric_linear_derivative(std::function<double(double)> f,
                                                      const double x,
                                                      const double delta);

    static double calculate_numeric_quadric_derivative(std::function<double(double)> f,
                                                       const double x,
                                                       const double delta);
};