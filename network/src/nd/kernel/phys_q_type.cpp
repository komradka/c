#include "phys_q_type.hpp"

std::string get_name_for_gui(phys_q_type type)
{
    switch (type)
    {
        case phys_q_type::gas_molar_rate: return "Gas Rate";
        case phys_q_type::pressure: return "Pressure";
        case phys_q_type::temperature: return "Temperature";
        case phys_q_type::water_molar_rate: return "Water Rate";
        case phys_q_type::oil_molar_rate: return "Oil Rate";
        case phys_q_type::COUNT: return "";
    }
    return "unknown";
}