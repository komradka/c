#include "report_enum.hpp"

std::string get_name(message_type e)
{
    switch (e)
    {
    case message_type::MESSAGE:
        return "Message: ";
    case message_type::WARNING:
        return "Warning: ";
    case message_type::ERR:
        return "Error: ";
    case message_type::DEBUG:
        return "D_Message: ";
    case message_type::COUNT:
        return "";
    }
    return "";
}

std::string get_name(message_source e)
{
    switch (e)
    {
    case message_source::MAIN:
        return "Project: ";
    case message_source::ND:
        return "Network Designer: ";
    case message_source::PIPE:
        return "Pipe Designer: ";
    case message_source::PVT:
        return "PVT Designer: ";
    case message_source::SIM:
        return "Simulator: ";
    case message_source::WF:
        return "Workflow: ";
    case message_source::COUNT:
        return "";
    }
    return "";
}
