
#pragma once

struct row_element
{
    double value;
    unsigned int pos;

    row_element(double value, unsigned int pos)
    {
        this->value = value;
        this->pos = pos;
    }
};