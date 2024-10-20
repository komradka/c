#include "reader.hpp"

/*
    TOPOLOGY:

    OBJECT TYPE:
        SOURCE
        SINK

    MAKE_OBJECT (OBJECT TYPE) (FILE WITH OBJECT DATA)

    MAKE_LINK (OBJECT NAME FIRST) (OBJECT NAME SECOND)
*/

error make_object_handler(const std::vector<std::string> &words, const int line, const string &filename, graph *topology)
{
    if (words.size() != 3)
        return error(make_error("Wrong number of fields", line), filename);

    auto type = words[1];
    auto data_file_name = words[2];

    error ret = topology->make_object(type, data_file_name);

    ret.set_filename(data_file_name);
    return ret;
}

error make_link_handler(const std::vector<std::string> &words, const int line, const string &filename, graph *topology)
{
    if (words.size() != 3)
        return error(make_error("Wrong number of fields", line), filename);
    
    auto first = words[1];
    auto second = words[2];

    error ret = topology->make_link(first, second);

    ret.set_filename(filename);
    return ret;
}

error reader::read_topology_string(const std::string str, const int line, graph *topology)
{
    std::vector<std::string> words;
    split_string(str, words);

    if (words.size() < 1)
        return error(make_error("Wrong number of fields", line), file_name);
    std::string main_word = words[0];

    if (topology_key_words.find(main_word) != topology_key_words.end())
        return topology_key_words[main_word](words, line, file_name, topology);

    return error(make_error("Unknown word: " + main_word, line), file_name);
}

void reader::init_topology_key_words()
{
    topology_key_words["MAKE_OBJECT"] = make_object_handler;
    topology_key_words["MAKE_LINK"] = make_link_handler;
}