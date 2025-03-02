#include "report_widget.hpp"

report_widget::report_widget(QWidget *parent) : QTextEdit(parent)
{
    setReadOnly(true);
    setLineWrapMode(QTextEdit::NoWrap);
    setAlignment(Qt::AlignTop);
}

void report_widget::print_message(std::string message)
{
    setTextColor(msg_color);
    QString msg = QString::fromStdString(message);
    append(msg);
}

void report_widget::print_warning(std::string message)
{
    setTextColor(war_color);
    QString msg = QString::fromStdString(message);
    append(msg);
}

void report_widget::print_error(std::string message)
{
    setTextColor(err_color);
    QString msg = QString::fromStdString(message);
    append(msg);
}

#include "moc_report_widget.cpp"
