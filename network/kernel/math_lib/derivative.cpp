#include "math_func.hpp"

double Math::calculate_numeric_linear_derivative(std::function<double(double)> f,
                                                 const double x,
                                                 const double delta)
{
    return (f(x + delta) - f(x)) / delta;
}

double Math::calculate_numeric_quadric_derivative(std::function<double(double)> f,
                                                  const double x,
                                                  const double delta)
{
    return (f(x + delta) - f(x - delta)) / (2 * delta);
}

double Math::calculate_numeric_derivative(std::function<double(double)> f,
                                          const double x,
                                          const double delta,
                                          const derivative_type type)
{
    return type == derivative_type::linear ? calculate_numeric_linear_derivative(f, x, delta) : calculate_numeric_quadric_derivative(f, x, delta);
}