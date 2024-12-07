#include "reader.hpp"

/*
    TOPOLOGY:

    OBJECT TYPE:
        SOURCE
        SINK

    MAKE_OBJECT (OBJECT TYPE) (FILE WITH OBJECT DATA)

    MAKE_LINK (OBJECT NAME FIRST) (OBJECT NAME SECOND)
*/

error make_object_handler(const std::vector<std::string> &words, const int line, const string &filename, graph_area *gui_manager)
{
    if (words.size() != 5)
    {
        for (int i = 0; i < words.size(); i++)
        {
            cout << words[i] << endl;
        }
        return error(make_error("Wrong number of fields", line), filename);
    }

    auto type = words[1];
    auto x_str = words[2];
    auto y_str = words[3];
    auto data_file_name = words[4];

    int x, y;

    sscanf(x_str.c_str(), "%d", &x);
    sscanf(y_str.c_str(), "%d", &y);

    error ret(OK);

    ret = gui_manager->add_object(type, QPoint(x, y), data_file_name);
    return ret;
}

error make_link_handler(const std::vector<std::string> &words, const int line, const string &filename, graph_area *gui_manager)
{
    (void)gui_manager;
    if (words.size() != 3)
        return error(make_error("Wrong number of fields", line), filename);

    auto first = words[1];
    auto second = words[2];

    gui_manager->add_link(first, second);

    // error ret = topology->make_link(first, second);

    // ret.set_filename(filename);
    return error(OK);
}

error reader::read_topology_string(const std::string str, const int line, graph_area *gui_manager)
{
    std::vector<std::string> words;
    split_string(str, words);

    if (words.size() < 1)
    {
        return error(make_error("Wrong number of fields", line), file_name);
    }
    std::string main_word = words[0];

    if (topology_key_words.find(main_word) != topology_key_words.end())
        return topology_key_words[main_word](words, line, file_name, gui_manager);

    return error(make_error("Unknown word: " + main_word, line), file_name);
}

void reader::init_topology_key_words()
{
    topology_key_words["MAKE_OBJECT"] = make_object_handler;
    topology_key_words["MAKE_LINK"] = make_link_handler;
}