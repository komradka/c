#include "initial_approximation_builder.hpp"

error initial_approximation_builder::simple_temp_initial_approximation()
{
    return error(OK);
}

error initial_approximation_builder::construct_temp_initial_approximation(bool is_simple_ia)
{
    if (is_simple_ia)
        return simple_temp_initial_approximation();

    return error(OK);
}