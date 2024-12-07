#include <vector>
#include "row_element.hpp"

#pragma once

struct jacobian_row
{
    static constexpr unsigned int pressure_pos = 0;
    static constexpr unsigned int rate_pos = 1;
    static constexpr unsigned int temp_pos = 2;

public:
    unsigned int size = 0;
    std::vector<row_element> row;

    unsigned int pos;
    unsigned int main_var_count;

public:
    jacobian_row(unsigned int pos, unsigned int var_count)
    {
        this->pos = pos;
        this->main_var_count = var_count;
    }

    void fill_row(unsigned int link,
                  double *p,
                  double *rate,
                  double *t)
    {
        unsigned int pos = link * main_var_count;
        auto add_element = [&](double value, unsigned int shift)
        {
            size++;
            row_element element(value, pos + shift);
            row.push_back(element);
        };

        if (p)
            add_element(*p, pressure_pos);
        if (rate)
            add_element(*rate, rate_pos);
        if (t)
            add_element(*t, temp_pos);
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
        fill_row(link_in, p_in, rate_in, t_in);
        fill_row(link_out, p_out, rate_out, t_out);
    }

    void fill_row_for_joint(std::vector<unsigned int> links, unsigned int link_in_count, unsigned int link_out_count)
    {
        auto get_pos = [&](unsigned int link) -> unsigned int
        {
            return link * main_var_count + rate_pos;
        };

        for (unsigned int i = 0; i < link_in_count; i++)
        {
            size++;
            row_element element(1., get_pos(links[i]));
            row.push_back(element);
        }
        for (unsigned int i = link_in_count; i < link_in_count + link_out_count; i++)
        {
            size++;
            row_element element(-1., get_pos(links[i]));
            row.push_back(element);
        }
    }
};