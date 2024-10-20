#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "objects_type.hpp"
#include "phys_q.hpp"
#include "objects_data/source_data.hpp"
#include "objects_data/sink_data.hpp"
#include "../reporter.hpp"
#include "../error.hpp"

#ifndef TOPOLOGY

#define TOPOLOGY

using object_id = int;
using link_id = int;

class vertex
{
private:
    network_objects type;
    object_data *data;
    std::vector<object_id> connected_objects;
    std::vector<link_id> links;
    object_id id;
    bool active;

public:
    vertex(network_objects type, object_id id) : type(type), id(id)
    {
    }
    void connect(object_id obj, link_id link)
    {
        connected_objects.push_back(obj);
        links.push_back(link);
    }
    const std::vector<object_id> &get_neighbour_objects() const
    {
        return connected_objects;
    }
    const std::vector<link_id> &get_links() const
    {
        return links;
    }
    const network_objects get_type() const
    {
        return type;
    }
    const int get_count_connected_vertex() const
    {
        return connected_objects.size();
    }
    const bool get_activity()
    {
        return active;
    }
    object_data *get_data()
    {
        return data;
    }

    const object_id get_id()
    {
        return id;
    }

    void set_data(object_data *data)
    {
        this->data = data;
    }

    ~vertex() = default;
};

class link
{
private:
    link_id id;
    std::pair<object_id, object_id> connected_objects;
    phys_q results;

public:
    link(const link_id id) : id(id) {}
    link(const link_id id, object_id first, object_id second) : id(id)
    {
        connected_objects = std::make_pair(first, second);
    }
    void connect(object_id obj1, object_id obj2)
    {
        connected_objects = std::make_pair(obj1, obj2);
    }
    const std::pair<object_id, object_id> &get_connected_objects() const
    {
        return connected_objects;
    }
};

class graph
{
private:
    reporter *rep;

    int vertex_count = 0;
    int links_count = 0;

    std::map<object_id, vertex *> vertices;
    std::map<link_id, link *> links;

    std::vector<object_id> active_objects;
    std::vector<object_id> inactive_objects;

private: // dates
    std::map<object_id, source_data> sources_data;
    std::map<object_id, sink_data> sinks_data;

public:
    graph(reporter *rep)
    {
        this->rep = rep;
    }
    ~graph()
    {
        for (auto v : vertices)
        {
            if (v.second)
                delete v.second;
        }
        for (auto l : links)
        {
            if (l.second)
                delete l.second;
        }
    }
    vertex *create_vertex(network_objects type);
    link *create_link(object_id first, object_id second);

    std::vector<object_id> get_objects() const
    {
        std::vector<object_id> result;
        for (const auto &v : vertices)
        {
            result.push_back(v.first);
        }
        return result;
    }

    std::vector<link_id> get_links() const
    {
        std::vector<object_id> result;
        for (const auto &l : links)
        {
            result.push_back(l.first);
        }
        return result;
    }

    // void add_result(const link_id link, const phys_q &result)
    // {
    // }

    std::vector<object_id> &get_active_objects()
    {
        return active_objects;
    }

    network_objects get_type(const object_id obj) const
    {
        auto v = vertices.find(obj);
        if (v != vertices.end())
            return v->second->get_type();
        return network_objects::unknown;
    }

    std::vector<object_id> get_active_objects_by_type(network_objects type) const
    {
        std::vector<object_id> result;
        for (auto obj : active_objects)
        {
            if (vertices.at(obj)->get_type() == type)
            {
                result.push_back(obj);
            }
        }
        return result;
    }

    std::pair<object_id, object_id> get_connected_object(link_id link)
    {
        return links[link]->get_connected_objects();
    }

    error make_object(const std::string &type, const std::string &file, vertex **v_ret = nullptr)
    {
        network_objects obj = string_to_type(type);

        vertex *v = create_vertex(obj);

        switch (obj)
        {
        case network_objects::source:
            RETURN_IF_FAIL(make_source(v, file));
            break;
        case network_objects::sink:
            RETURN_IF_FAIL(make_sink(v, file));
            break;
        case network_objects::unknown:
            return error("Unknown object");
        }

        rep->print_message(get_name_for_gui(obj) + ": " + v->get_data()->name + " - Successfully created");

        if (v_ret)
            *v_ret = v;

        return error(OK);
    }

    error make_link(std::string &first, std::string &second)
    {
        auto obj1 = get_object_by_name(first);
        auto obj2 = get_object_by_name(second);

        if (!obj1)
            return error("Cannot find object: " + first);
        if (!obj2)
            return error("Cannot find object: " + second);

        link *l = create_link(obj1.value(), obj2.value());

        rep->print_message("Link between " + first + " and " + second + " - Successfully created");
        return error(OK);
    }

private:
    error make_source(vertex *v, const std::string &file);
    error make_sink(vertex *v, const std::string &file);

    vertex *get_objects(const object_id obj)
    {
        auto v = vertices.find(obj);
        if (v != vertices.end())
            return v->second;
        return nullptr;
    }

    std::optional<object_id> get_object_by_name(std::string &name)
    {
        for (auto v : vertices)
        {
            if (v.second->get_data()->name == name)
                return make_optional(v.first);
        }
        return nullopt;
    }

    link *get_link(const link_id link)
    {
        auto l = links.find(link);
        if (l != links.end())
            return l->second;
        return nullptr;
    }

    std::string gen_name(network_objects type)
    {
        switch (type)
        {
        case network_objects::source:
            return get_name_for_gui(type) + " " + to_string(sources_data.size() + 1);
        case network_objects::sink:
            return get_name_for_gui(type) + " " + to_string(sinks_data.size() + 1);
        case network_objects::unknown:
            return "error object";
        }
        return "error";
    }
};

#endif