#include <string>

#ifndef QT

#define QT

enum class phys_q_type
{
    pressure,
    temperature,
    gas_molar_rate,
    oil_molar_rate,
    water_molar_rate,

    unknown
};

std::string get_name_for_gui(phys_q_type &obj);

#endif 