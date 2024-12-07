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
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, object_data *)>> source_keywords;
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, object_data *)>> sink_keywords;
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, object_data *)>> joint_keywords;
    std::map<std::string, std::function<error(const std::vector<std::string> &, const int, object_data *)>> pipe_keywords;
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
            init_pipe_keywords();
            break;
        case network_objects::joint:
            init_joint_keywords();
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
                return error(make_object_error("Wrong number of fields", line_num), file_name);
            }

            if (source_keywords.find(words[1]) != source_keywords.end())
            {
                RETURN_IF_FAIL(source_keywords.at(words[1])(words, line_num, &data));
            }
            else
            {
                return error(make_object_error("Unknown word: " + words[1], line_num));
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
                return error(make_object_error("Wrong number of fields", line_num), file_name);
            }

            if (sink_keywords.find(words[1]) != sink_keywords.end())
            {
                RETURN_IF_FAIL(sink_keywords.at(words[1])(words, line_num, &data));
            }
            else
            {
                return error(make_object_error("Unknown word: " + words[1], line_num));
            }

            line_num++;
        }

        file.close();
        return error("Wrong number of fields", 0);
    }

    error joint_handler(const std::string &file_name, joint_data &data)
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
                return error(make_object_error("Wrong number of fields", line_num), file_name);
            }

            if (joint_keywords.find(words[1]) != joint_keywords.end())
            {
                RETURN_IF_FAIL(joint_keywords.at(words[1])(words, line_num, &data));
            }
            else
            {
                return error(make_object_error("Unknown word: " + words[1], line_num));
            }

            line_num++;
        }

        file.close();
        return error("Wrong number of fields", 0);
    }

    error pipe_handler(const std::string &file_name, pipe_data &data)
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
                return error(make_object_error("Wrong number of fields", line_num), file_name);
            }

            if (pipe_keywords.find(words[1]) != pipe_keywords.end())
            {
                RETURN_IF_FAIL(pipe_keywords.at(words[1])(words, line_num, &data));
            }
            else
            {
                return error(make_object_error("Unknown word: " + words[1], line_num));
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
        source_keywords["PRESSURE"] = set_pressure;
        source_keywords["TEMPERATURE"] = set_temperature;
        source_keywords["WRAT"] = set_wrat;
        source_keywords["ORAT"] = set_orat;
        source_keywords["GRAT"] = set_grat;
        source_keywords["ACTIVITY"] = set_activity;
        source_keywords["NAME"] = set_name;
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
        sink_keywords["PRESSURE"] = set_pressure;
        sink_keywords["TEMPERATURE"] = set_temperature;
        sink_keywords["WRAT"] = set_wrat;
        sink_keywords["ORAT"] = set_orat;
        sink_keywords["GRAT"] = set_grat;
        sink_keywords["ACTIVITY"] = set_activity;
        sink_keywords["NAME"] = set_name;
    }

    void init_joint_keywords()
    {
        joint_keywords["NAME"] = set_name;
        joint_keywords["ACTIVITY"] = set_activity;
    }

    void init_pipe_keywords()
    {
        pipe_keywords["NAME"] = set_name;
        pipe_keywords["ACTIVITY"] = set_activity;
        pipe_keywords["PIPE_TYPE"] = set_pipe_type;
        pipe_keywords["PIPE_LEN"] = set_pipe_len;
        pipe_keywords["PIPE_P_COEF"] = set_pipe_pressure_drop_coef;
        pipe_keywords["PIPE_T_COEF"] = set_pipe_temp_drop_coef;
        pipe_keywords["PIPE_H_DIFF"] = set_pipe_height_diff;
        pipe_keywords["PIPE_D_IN"] = set_pipe_inner_diameter;
        pipe_keywords["PIPE_ROUGH"] = set_pipe_roughness;
        pipe_keywords["PIPE_DEPTH"] = set_pipe_burial_depth;
        pipe_keywords["PIPE_COND"] = set_pipe_wall_cond;
        pipe_keywords["PIPE_THK"] = set_pipe_wall_thickness;
        pipe_keywords["PIPE_TA"] = set_ambient_temperature;
        pipe_keywords["PIPE_HTC"] = set_htc;
    }
};