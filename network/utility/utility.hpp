#include <string>

#pragma once

template <auto Start, auto End, auto Inc = 1, typename F>
constexpr void constexpr_for(F &&f)
{
    static_assert(Inc != 0);
    if constexpr ((Inc > 0 && Start < End) || (Inc < 0 && End < Start))
    {
        f(std::integral_constant<decltype(Start), Start>());
        constexpr_for<Start + Inc, End, Inc>(f);
    }
}

std::string get_name_for_gui(int e);

std::string get_name_for_gui(double e);

std::string get_name_for_gui(bool e);