#include "phys_q_type.hpp"

std::string get_name_for_gui(phys_q_type &type)
{
    switch (type)
    {
        case phys_q_type::gas_molar_rate: return "gas molar rate";
        case phys_q_type::pressure: return "pressure";
        case phys_q_type::temperature: return "temperature";
        case phys_q_type::water_molar_rate: return "water molar rate";
        case phys_q_type::oil_molar_rate: return "oil molar rate";
    }
    return "unknown";
}