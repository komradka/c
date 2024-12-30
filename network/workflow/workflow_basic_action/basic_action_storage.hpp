#include "basic_action.hpp"
#include <array>
#pragma once

struct basic_action_storage
{
    static constexpr int basic_action_count = 1;

    std::array<abstract_basic_action *, basic_action_count> basic_action;

    basic_action_storage()
    {
        basic_action[0] = new make_object_action();
    }
};