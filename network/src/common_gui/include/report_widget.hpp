#include <QtWidgets/QtWidgets>
#include <iostream>
#include <string>
#include "slot.hpp"

#pragma once

class report_widget : public QTextEdit
{
    Q_OBJECT

private:
    QColor msg_color = QColor(Qt::blue);
    QColor err_color = QColor(Qt::red);
    QColor war_color = QColor(Qt::yellow);

public:
    report_widget(QWidget *parent);

    void print_message(std::string message);
    void print_warning(std::string message);
    void print_error(std::string message);

    Slots m_slot;
};
