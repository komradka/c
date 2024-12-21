#include "initial_approximation_builder.hpp"

error initial_approximation_builder::simple_temp_initial_approximation(const thread_info &)
{
    return error(OK);
}

error initial_approximation_builder::construct_temp_initial_approximation(bool is_simple_ia, const thread_info &thr_info)
{
    if (is_simple_ia)
        return simple_temp_initial_approximation(thr_info);

    thr_info.barrier ();
    return error(OK);
}
