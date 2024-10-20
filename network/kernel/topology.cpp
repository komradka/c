#include "topology.hpp"
#include "../reader/objects_data_reader.hpp"

vertex *graph::create_vertex(network_objects type)
{
    vertex *new_v = new vertex(type, vertex_count);
    vertices[vertex_count] = new_v;
    vertex_count++;
    return new_v;
}

link *graph::create_link(object_id first, object_id second)
{
    link *new_l = new link(links_count, first, second);
    links[links_count] = new_l;
    links_count++;
    return new_l;
}

error graph::make_source(vertex *v, const std::string &file)
{
    source_data data;

    data_reader r(rep, network_objects::source);

    RETURN_IF_FAIL(r.source_handler(file, data));

    if (data.name.empty())
    {
        data.name = gen_name(network_objects::source);
    }

    sources_data[v->get_id()] = data;

    v->set_data(&sources_data[v->get_id()]);

    return error(OK);
}

error graph::make_sink(vertex *v, const std::string &file)
{
    sink_data data;

    data_reader r(rep, network_objects::sink);

    RETURN_IF_FAIL(r.sink_handler(file, data));

    if (data.name.empty())
    {
        data.name = gen_name(network_objects::sink);
    }

    sinks_data[v->get_id()] = data;

    v->set_data(&sinks_data[v->get_id()]);

    return error(OK);
}