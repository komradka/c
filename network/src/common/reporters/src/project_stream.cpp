#include "project_stream.hpp"

project_stream::~project_stream()
{
    if (!print_to_widget && !print_to_stdout)
    {
        fclose(fp);
    }
}

std::string get_name(file_mode e)
{
    switch (e)
    {
    case file_mode::read:
        return "r";
    case file_mode::write:
        return "w";
    case file_mode::append:
        return "a";
    case file_mode::none:
    case file_mode::COUNT:
        return "";
    }
    return "";
}

error project_stream::open(const char *file, file_mode mode)
{
    if (file == nullptr)
    {
        fp = stdout;
        print_to_stdout = true;
    }
    else
    {
        fp = fopen(file, get_name(mode).c_str());
        if (!fp)
        {
            std::string file_name(file);
            return error("Cannot open file " + file_name);
        }
    }

    return error(OK);
}

error project_stream::open(report_widget *out_widget)
{
    if (out_widget == nullptr)
        return error("Cannot find text widget");

    out_widget->m_slot.connect_to(print_message_to_widget, [&](std::string str)
                                  { out_widget->print_message(str); });
    out_widget->m_slot.connect_to(print_warning_to_widget, [&](std::string str)
                                  { out_widget->print_warning(str); });
    out_widget->m_slot.connect_to(print_error_to_widget, [&](std::string str)
                                  { out_widget->print_error(str); });

    return error(OK);
}

void project_stream::print(std::string msg, int type)
{
    if (print_to_widget)
    {
        if (type == 0)
        {
            print_message_to_widget.Emit(msg);
        }
        else if (type == 1)
        {
            print_warning_to_widget.Emit(msg);
        }
        else if (type == 2)
        {
            print_error_to_widget.Emit(msg);
        }
        else
        {
            abort();
        }
    }
    else
    {
        fprintf(fp, "%s\n", msg.c_str());
    }
}