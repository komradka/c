#include "joint_data.hpp"

void joint_data::write_data(std::ofstream &stream)
{
    auto write_param = [&](auto i)
    {
        using data_type_t = typename network_object_param_desc<joint_data::joint_params[i]>::data_type_t;
        data_type_t value = get_param<joint_data::joint_params[i]>();

        if (value.has_value())
            stream << "SET " << network_object_param_desc<joint_data::joint_params[i]>::name << " " << value.value() << std::endl;
    };

    constexpr_for<0, joint_data::joint_fields>(write_param);
}