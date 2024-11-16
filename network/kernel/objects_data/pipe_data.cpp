#include "pipe_data.hpp"
#include <iostream>

void pipe_data::write_data(std::ofstream &stream)
{
    (void)stream;
    // auto write_param = [&](auto i)
    // {
    //     using data_type_t = typename network_object_param_desc<pipe_data::pipe_params[i]>::data_type_t;
    //     data_type_t value = get_param<pipe_data::pipe_params[i]>();

    //     if (value.has_value())
    //         stream << "SET " << network_object_param_desc<pipe_data::pipe_params[i]>::name << " " << value.value() << std::endl;
    // };

    // constexpr_for<0, pipe_data::pipe_fields>(write_param);
}

bool pipe_data::add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count)
{
    std::cout << "in " << inlet_links_count << std::endl;
    std::cout << "out " << outlet_links_count << std::endl;

    if (inlet_links_count > this->inlet_links_count || outlet_links_count > this->outlet_links_count)
        return false;
    return true;
}
