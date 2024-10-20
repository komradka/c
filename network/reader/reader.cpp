#include "reader.hpp"

void split_string(const std::string &str, std::vector<std::string> &words)
{
    std::stringstream ss(str);
    std::string word;
    while (ss >> word)
    {
        words.push_back(word);
    }
}

std::string make_error(const std::string &err_message, const int line)
{
    return "Wrong file format: " + err_message + ".\n" + "In line - " + std::to_string(line);
}