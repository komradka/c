#include "graph_area.hpp"

it *source_creator(vertex *v)
{
    source_item *item = new source_item();
    item->set_object(v);

    return item;
}

it *sink_creator(vertex *v)
{
    sink_item *item = new sink_item();
    item->set_object(v);

    return item;
}