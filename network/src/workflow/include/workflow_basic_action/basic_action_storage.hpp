#include "basic_action.hpp"
#include <array>
#pragma once

struct basic_action_storage
{
    std::vector<abstract_basic_action *> basic_action;

    basic_action_storage();
};