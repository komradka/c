#include "signal.hpp"

#pragma once

struct Slots
{
    template <typename... Args, typename FUNC>
    void connect_to(Signal<Args...> &s, FUNC &&f)
    {
        s.Connect(std::forward<FUNC>(f));
    }
};