#include "reporter.hpp"

void reporter::print_message(std::string message)
{
    setTextColor(msg_color);
    QString msg = ("Message: " + message).c_str();
    append(msg);
}

void reporter::print_debug_message(std::string message)
{
    if (!is_debug)
        return;
    print_message(message);
}

void reporter::print_warning(std::string message)
{
    stat->add_warning();
    setTextColor(war_color);
    QString msg = ("Warning: " + message).c_str();
    append(msg);
}

void reporter::print_error(std::string message)
{
    stat->add_error();
    setTextColor(err_color);
    QString msg = ("Error: " + message).c_str();
    append(msg);
}