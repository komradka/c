#include "basic_action_storage.hpp"

basic_action_storage::basic_action_storage()
{
    basic_action.push_back(new make_object_action());
    basic_action.push_back(new make_link_action());
    basic_action.push_back(new get_object_action());
}