#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include <QtWidgets/QtWidgets>

#include "objects_type.hpp"
#include "objects_data/source_data.hpp"
#include "objects_data/sink_data.hpp"
#include "objects_data/pipe_data.hpp"
#include "objects_data/joint_data.hpp"
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

    std::vector<link_id> inlet_links;
    std::vector<link_id> outlet_links;

    object_id id;
    bool active;

public:
    vertex(network_objects type, object_id id) : type(type), id(id)
    {
    }
    void connect(link_id link, link_direction dir)
    {
        dir == link_direction::inlet ? inlet_links.push_back(link) : outlet_links.push_back(link);
    }

    void delete_in_link(link_id l)
    {
        auto it = std::find(inlet_links.begin(), inlet_links.end(), l);

        if (it != inlet_links.end())
            inlet_links.erase(it);
    }

    void delete_out_link(link_id l)
    {
        auto it = std::find(outlet_links.begin(), outlet_links.end(), l);

        if (it != outlet_links.end())
            outlet_links.erase(it);
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
    std::map<object_id, joint_data> joints_data;

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

    std::optional<link_id> get_link_between_object(object_id f, object_id s)
    {
        std::vector<link_id> links = get_links();

        for (auto l : links)
        {
            std::pair<object_id, object_id> co = get_connected_object(l);
            if ((co.first == f && co.second == s) ||
                (co.first == s && co.second == f))
                return make_optional(l);
        }

        return nullopt;
    }

    std::vector<link_id> get_object_links(object_id obj)
    {
        auto inlet = get_inlet_links(obj);
        auto outlet = get_outlet_links(obj);

        std::vector<link_id> res;

        for (link_id l : inlet)
        {
            res.push_back(l);
        }

        for (link_id l : outlet)
        {
            res.push_back(l);
        }

        return res;
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
        case network_objects::joint:
            RETURN_IF_FAIL(make_joint(v, file));
            break;
        case network_objects::link:
        case network_objects::unknown:
        case network_objects::COUNT:
            return error("Unknown object");
        }

        rep->print_message(get_name_for_gui(obj) + ": " + v->get_data()->get_name() + " - Successfully created");

        if (v_ret)
            *v_ret = v;

        update_active_objects();
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

    pipe_data *get_pipe_data(object_id pipe)
    {
        if (get_type(pipe) != network_objects::pipe)
            return nullptr;

        auto data = get_object_data(pipe);

        return dynamic_cast<pipe_data *>(data);
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

    void remove_link_at_object(object_id obj, link_id link)
    {
        vertex *v = get_object(obj);

        v->delete_in_link(link);
        v->delete_out_link(link);
    }

    void delete_object(object_id obj)
    {
        vertex *v = vertices.at(obj);

        network_objects type = get_type(obj);

        std::vector<link_id> in_links = v->get_inlet_links();
        std::vector<link_id> out_links = v->get_outlet_links();

        for (link_id l : in_links)
        {
            object_id rhs;
            std::tie(rhs, std::ignore) = get_connected_object(l);

            remove_link_at_object(rhs, l);

            link *_l = links[l];

            links.erase(l);

            delete _l;
        }

        for (link_id l : out_links)
        {
            object_id rhs;
            std::tie(std::ignore, rhs) = get_connected_object(l);

            remove_link_at_object(rhs, l);

            link *_l = links[l];

            links.erase(l);

            delete _l;
        }

        vertices.erase(obj);
        delete v;

        switch (type)
        {
        case network_objects::source:
        {
            sources_data.erase(obj);
            break;
        }
        case network_objects::sink:
        {
            sinks_data.erase(obj);
            break;
        }
        case network_objects::pipe:
        {
            pipes_data.erase(obj);
            break;
        }
        case network_objects::joint:
        {
            joints_data.erase(obj);
            break;
        }
        case network_objects::link:
        case network_objects::unknown:
        case network_objects::COUNT:
            break;
        }

        update_active_objects();
    }

    void delete_link(link_id l)
    {
        std::pair<object_id, object_id> co = get_connected_object(l);

        remove_link_at_object(co.first, l);
        remove_link_at_object(co.second, l);

        link *_l = links.at(l);

        links.erase(l);

        delete _l;
    }

    std::vector<object_id> get_neighbors(object_id obj)
    {
        std::vector<object_id> res;

        std::vector<link_id> in = get_inlet_links(obj);

        for (link_id l : in)
        {
            object_id rhs;
            std::tie(rhs, std::ignore) = get_connected_object(l);
            res.push_back(rhs);
        }

        std::vector<link_id> out = get_outlet_links(obj);

         for (link_id l : out)
        {
            object_id rhs;
            std::tie(std::ignore, rhs) = get_connected_object(l);
            res.push_back(rhs);
        }
        return res;
    }

private:
    error make_source(vertex *v, const std::string &file);
    error make_sink(vertex *v, const std::string &file);
    error make_pipe(vertex *v, const std::string &file);
    error make_joint(vertex *v, const std::string &file);

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
            return get_name_for_gui(type) + "_" + to_string(sources_data.size() + 1);
        case network_objects::sink:
            return get_name_for_gui(type) + "_" + to_string(sinks_data.size() + 1);
        case network_objects::pipe:
            return get_name_for_gui(type) + "_" + to_string(pipes_data.size() + 1);
        case network_objects::joint:
            return get_name_for_gui(type) + "_" + to_string(joints_data.size() + 1);
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
