#include <map>
#include <functional>
#include <fstream>
#include <sstream>

#include "../kernel/objects_type.hpp"
#include "../kernel/objects_data/sink_data.hpp"
#include "../kernel/objects_data/source_data.hpp"
#include "../error.hpp"
#include "reader.hpp"

#pragma once

class data_reader
{
private:
    reporter *rep;
    network_objects type;
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, source_data &)>> source_keywords;
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, sink_data &)>> sink_keywords;
    // std::map<std::string, std::function<error(const std::vector<std::string> &, const int, sink_data &data)>> sink_keywords;
public:
    data_reader(reporter *r, network_objects type)
    {
        rep = r;
        this->type = type;
        switch (type)
        {
        case network_objects::source:
            init_source_keywords();
            break;
        case network_objects::sink:
            init_sink_keywords();
            break;
        case network_objects::pipe:
            break;
        case network_objects::link:
        case network_objects::unknown:
        case network_objects::COUNT:
            return;
        }
    }

public: // handlers
    error source_handler(const std::string &file_name, source_data &data)
    {
        std::ifstream file;

        file.open(file_name);
        if (!file.is_open())
        {
            return error("Unable to open file", file_name);
        }

        int line_num = 1;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::vector<std::string> words;
            split_string(line, words);

            if (words.size() < 2)
            {
                file.close();
                return error(make_source_error("Wrong number of fields", line_num), file_name);
            }

            if (source_keywords.find(words[1]) != source_keywords.end())
            {
                RETURN_IF_FAIL(source_keywords.at(words[1])(words, line_num, data));
            }
            else
            {
                return error(make_source_error("Unknown word: " + words[1], line_num));
            }

            line_num++;
        }

        file.close();
        return error("Wrong number of fields", 0);
    }

    error sink_handler(const std::string &file_name, sink_data &data)
    {
        std::ifstream file;

        file.open(file_name);
        if (!file.is_open())
        {
            return error("Unable to open file", file_name);
        }

        int line_num = 1;

        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> words;
            split_string(line, words);

            if (words.size() < 2)
            {
                file.close();
                return error(make_sink_error("Wrong number of fields", line_num), file_name);
            }

            if (sink_keywords.find(words[1]) != sink_keywords.end())
            {
                RETURN_IF_FAIL(sink_keywords.at(words[1])(words, line_num, data));
            }
            else
            {
                return error(make_sink_error("Unknown word: " + words[1], line_num));
            }

            line_num++;
        }

        file.close();
        return error("Wrong number of fields", 0);
    }

private: // init
    /*
        SOURCE DATA KEYWORDS:
        SET
            NAME (STRING)
            PRESSURE (NUM)
            TEMPERATURE (NUM)
            WRAT (NUM)
            ORAT (NUM)
            GRAT (NUM)
            STATUS ACTIVE / INACTIVE
    */

    void init_source_keywords()
    {
        source_keywords["PRESSURE"] = source_set_pressure;
        source_keywords["TEMPERATURE"] = source_set_temperature;
        source_keywords["WRAT"] = source_set_wrat;
        source_keywords["ORAT"] = source_set_orat;
        source_keywords["GRAT"] = source_set_grat;
        source_keywords["ACTIVITY"] = source_set_activity;
        source_keywords["NAME"] = source_set_name;
    }

    /*
        SINK DATA KEYWORDS:
        SET
            NAME (STRING)
            PRESSURE (NUM)
            TEMPERATURE (NUM)
            WRAT (NUM)
            ORAT (NUM)
            GRAT (NUM)
            STATUS ACTIVE / INACTIVE
    */

    void init_sink_keywords()
    {
        sink_keywords["PRESSURE"] = sink_set_pressure;
        sink_keywords["TEMPERATURE"] = sink_set_temperature;
        sink_keywords["WRAT"] = sink_set_wrat;
        sink_keywords["ORAT"] = sink_set_orat;
        sink_keywords["GRAT"] = sink_set_grat;
        sink_keywords["ACTIVITY"] = sink_set_activity;
        sink_keywords["NAME"] = sink_set_name;
    }
};