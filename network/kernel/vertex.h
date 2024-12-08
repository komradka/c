#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include "objects_type.hpp"
#include "objects_data/object_data.hpp"

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

    std::vector<object_id> get_neighbour_objects()
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

#endif // VERTEX_H
