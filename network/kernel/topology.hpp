#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include <QtWidgets/QtWidgets>

#include "objects_type.hpp"
#include "objects_data/source_data.hpp"
#include "objects_data/sink_data.hpp"
#include "objects_data/pipe_data.hpp"
#include "../reporter.hpp"
#include "../error.hpp"
#include "check_network.hpp"

#pragma once

using object_id = int;
using link_id = int;

enum class link_direction
{
    inlet,
    outlet,
};

class vertex
{
private:
    network_objects type;
    object_data *data;
    std::vector<object_id> connected_objects;

    std::vector<link_id> inlet_links;
    std::vector<link_id> outlet_links;

    object_id id;
    bool active;

public:
    vertex(network_objects type, object_id id) : type(type), id(id)
    {
    }
    void connect(object_id obj, link_id link, link_direction dir)
    {
        connected_objects.push_back(obj);
        dir == link_direction::inlet ? inlet_links.push_back(link) : outlet_links.push_back(link);
    }
    std::vector<object_id> &get_neighbour_objects()
    {
        return connected_objects;
    }
    std::vector<link_id> &get_inlet_links()
    {
        return inlet_links;
    }
    std::vector<link_id> &get_outlet_links()
    {
        return outlet_links;
    }
    network_objects get_type()
    {
        return type;
    }
    int get_count_connected_vertex()
    {
        return connected_objects.size();
    }
    bool get_activity()
    {
        return active;
    }
    object_data *get_data()
    {
        return data;
    }

    object_id get_id()
    {
        return id;
    }

    void set_data(object_data *data)
    {
        this->data = data;
    }

    bool add_link_verification(link_direction new_link_dir)
    {
        return data->add_link_verification(inlet_links.size() + (new_link_dir == link_direction::inlet ? 1 : 0),
                                           outlet_links.size() + (new_link_dir == link_direction::outlet ? 1 : 0));
    }

    ~vertex() = default;
};

class link
{
private:
    link_id id;
    std::pair<object_id, object_id> connected_objects;

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
    std::pair<object_id, object_id> &get_connected_objects()
    {
        return connected_objects;
    }
    link_id get_id()
    {
        return id;
    }
};

class graph : public QObject
{
    Q_OBJECT

public slots:
    void update_active_objects()
    {
        active_objects.clear();
        for (auto &v : vertices)
        {
            if (v.second->get_data()->is_active())
            {
                active_objects.push_back(v.first);
            }
        }
    }

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
    std::map<object_id, pipe_data> pipes_data;

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

    std::vector<object_id> get_objects()
    {
        std::vector<object_id> result;
        for (const auto &v : vertices)
        {
            result.push_back(v.first);
        }
        return result;
    }

    std::vector<link_id> get_links()
    {
        std::vector<object_id> result;
        for (const auto &l : links)
        {
            result.push_back(l.first);
        }
        return result;
    }

    std::vector<object_id> &get_active_objects()
    {
        return active_objects;
    }

    network_objects get_type(const object_id obj)
    {
        auto v = vertices.find(obj);
        if (v != vertices.end())
            return v->second->get_type();
        return network_objects::unknown;
    }

    std::string get_object_name(const object_id obj)
    {
        vertex *v = get_object(obj);
        if (!v)
            return "";

        return v->get_data()->get_name();
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
        case network_objects::pipe:
            RETURN_IF_FAIL(make_pipe(v, file));
            break;
        case network_objects::link:
        case network_objects::unknown:
        case network_objects::COUNT:
            return error("Unknown object");
        }

        rep->print_message(get_name_for_gui(obj) + ": " + v->get_data()->get_name() + " - Successfully created");

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
        (void)l;

        rep->print_message("Link between " + first + " and " + second + " - Successfully created");
        return error(OK);
    }

    std::optional<object_id> get_object_by_name(std::string &name)
    {
        for (auto v : vertices)
        {
            if (v.second->get_data() && v.second->get_data()->get_name() == name)
                return make_optional(v.first);
        }
        return nullopt;
    }

    vertex *get_object(const object_id obj)
    {
        auto v = vertices.find(obj);
        if (v != vertices.end())
            return v->second;
        return nullptr;
    }

    object_data *get_object_data(object_id obj)
    {
        vertex *v = get_object(obj);
        if (!v)
            return nullptr;
        return v->get_data();
    }

    source_data *get_source_data(object_id source)
    {
        if (get_type(source) != network_objects::source)
            return nullptr;

        auto data = get_object_data(source);

        return dynamic_cast<source_data *>(data);
    }

    sink_data *get_sink_data(object_id sink)
    {
        if (get_type(sink) != network_objects::sink)
            return nullptr;

        auto data = get_object_data(sink);

        return dynamic_cast<sink_data *>(data);
    }

public:
    std::vector<link_id> &get_inlet_links(object_id obj)
    {
        vertex *v = get_object(obj);

        return v->get_inlet_links();
    }

    std::vector<link_id> &get_outlet_links(object_id obj)
    {
        vertex *v = get_object(obj);

        return v->get_outlet_links();
    }

    error verify_network_before_calculation()
    {
        return check_objects(this);
    }

    std::vector<object_id> get_boundary_objects()
    {
        std::vector<object_id> res;
        std::vector<object_id> active_objs = get_active_objects();
        for (auto &obj : active_objs)
        {
            if (is_boundary_object(obj))
                res.push_back(obj);
        }
        return res;
    }

    bool is_boundary_object(object_id obj)
    {
        return (is_source_object(obj) || is_sink_object(obj));
    }

    bool is_sink_object(object_id obj)
    {
        network_objects type = get_type(obj);
        if (type == network_objects::sink)
            return true;
        return false;
    }

    bool is_source_object(object_id obj)
    {
        network_objects type = get_type(obj);
        if (type == network_objects::source)
            return true;
        return false;
    }

    int get_boundary_condition_count_on_object(object_id obj)
    {
        int res = 0;

        if (!is_boundary_object(obj))
            return 0;

        network_objects type = get_type(obj);

        vertex *v = get_object(obj);

        using p = network_object_param;

        if (type == network_objects::source)
        {
            source_data *data = dynamic_cast<source_data *>(v->get_data());
            res += data->get_param<p::pressure>().has_value() ? 1 : 0;
            res += data->get_param<p::wrat>().has_value() ? 1 : 0;
        }

        else if (type == network_objects::sink)
        {
            sink_data *data = dynamic_cast<sink_data *>(v->get_data());
            res += data->get_param<p::pressure>().has_value() ? 1 : 0;
            res += data->get_param<p::wrat>().has_value() ? 1 : 0;
        }

        return res;
    }

private:
    error make_source(vertex *v, const std::string &file);
    error make_sink(vertex *v, const std::string &file);
    error make_pipe(vertex *v, const std::string &file);

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
        case network_objects::pipe:
            return get_name_for_gui(type) + " " + to_string(pipes_data.size() + 1);
        case network_objects::link:
        case network_objects::unknown:
        case network_objects::COUNT:
            return "error object";
        }
        return "error";
    }

    bool add_object_verification(object_data &data)
    {
        std::string name = data.get_name();

        if (!name.empty() && get_object_by_name(name).has_value())
        {
            return false;
        }
        return true;
    }
};
