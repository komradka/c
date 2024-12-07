#include "sink_data.hpp"

void sink_data::write_data(std::ofstream &stream)
{
    auto write_param = [&](auto i)
    {
        using data_type_t = typename network_object_param_desc<sink_data::sink_params[i]>::data_type_t;
        data_type_t value = get_param<sink_data::sink_params[i]>();

        if (value.has_value())
            stream << "SET " << network_object_param_desc<sink_data::sink_params[i]>::name << " " << value.value() << std::endl;
    };

    constexpr_for<0, sink_data::sink_fields>(write_param);
}

bool sink_data::add_link_verification(const unsigned int inlet_links_count, const unsigned int outlet_links_count)
{
    if (inlet_links_count > this->inlet_links_count || outlet_links_count > this->outlet_links_count)
        return false;
    return true;
}
