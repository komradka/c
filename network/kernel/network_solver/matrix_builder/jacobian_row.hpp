#include <vector>
#include "row_element.hpp"

struct jacobian_row
{
    static constexpr unsigned int pressure_pos = 0;
    static constexpr unsigned int rate_pos = 1;
    static constexpr unsigned int temp_pos = 2;

public:
    unsigned int size = 0;
    std::vector<row_element> row;

    unsigned int pos;
    unsigned int main_value_count;

public:
    jacobian_row(unsigned int pos, unsigned int main_value_count)
    {
        this->pos = pos;
        this->main_value_count = main_value_count;
    }

    void fill_row(unsigned int link_in,
                  unsigned int link_out,
                  double *p_in,
                  double *p_out,
                  double *rate_in,
                  double *rate_out,
                  double *t_in,
                  double *t_out)
    {
        unsigned int in_pos_start = link_in * main_value_count;
        unsigned int out_pos_start = link_out * main_value_count;

        auto add_element = [&](double value, bool in, unsigned int shift)
        {
            size++;
            row_element element(value, (in ? in_pos_start : out_pos_start) + shift);
            row.push_back(element);
        };

        if (p_in)
        {
            add_element(*p_in, true, pressure_pos);
        }

        if (p_out)
        {
            add_element(*p_out, false, pressure_pos);
        }

        if (rate_in)
        {
            add_element(*rate_in, true, rate_pos);
        }

        if (rate_out)
        {
            add_element(*rate_out, false, rate_pos);
        }

        if (t_in)
        {
            add_element(*t_in, true, temp_pos);
        }

        if (t_out)
        {
            add_element(*t_out, false, temp_pos);
        }

    }
};