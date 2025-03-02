#include "report_widget.hpp"
#include "error.hpp"
#include "signal.hpp"

enum class file_mode
{
    read,
    write,
    append,
    none,

    COUNT
};

std::string get_name(file_mode e);

class project_stream
{
private:
    FILE *fp;
    bool print_to_widget = false;
    bool print_to_stdout = false;

public:
    project_stream() = default;

    ~project_stream();

    error open(const char *file = nullptr, file_mode mode = file_mode::none);
    error open(report_widget *out_widget);

    void print(std::string msg, int type = 0);

    Signal<std::string> print_message_to_widget;
    Signal<std::string> print_warning_to_widget;
    Signal<std::string> print_error_to_widget;
};