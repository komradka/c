#include "../../utility/table.hpp"
#include <functional>

#pragma once

enum class unit_type
{
    pressure,
    temperature,
    rate,

    COUNT
};

enum class pressure_unit
{
    bar,
    pascal,

    COUNT
};

enum class temperature_unit
{
    celsius,
    fahrenheit,
    kelvin,

    COUNT
};

class unit_manager
{
private:
    table<std::function<double(double)>> *pressure_converter;
    table<std::function<double(double)>> *temperature_converter;

public:
    unit_manager()
    {
        pressure_converter = new table<std::function<double(double)>>((unsigned int)pressure_unit::COUNT, (unsigned int)pressure_unit::COUNT);
        temperature_converter = new table<std::function<double(double)>>((unsigned int)temperature_unit::COUNT, (unsigned int)temperature_unit::COUNT);
    }

private:
    void init()
    {
        
    }
};