#include "graph_area.hpp"

it *source_creator(vertex *v, QWidget *gui_manager)
{
    source_item *item = new source_item(v, 0, gui_manager);

    return item;
}

it *sink_creator(vertex *v, QWidget *gui_manager)
{
    sink_item *item = new sink_item(v, 0, gui_manager);

    return item;
}

it *pipe_creator(vertex *v, QWidget *gui_manager)
{
    pipe_item *item = new pipe_item(v, 0, gui_manager);

    return item;
}

it *joint_creator(vertex *v, QWidget *gui_manager)
{
    joint_item *item = new joint_item(v, 0, gui_manager);

    return item;
}