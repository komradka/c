#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>
#include <iostream>

#include "utility/network_statistic.hpp"

#pragma once

using namespace std;

class reporter : public QTextEdit
{
    Q_OBJECT
private:
    bool is_debug;
    network_statistic *stat;

    QScrollBar *sb;
    QColor msg_color = QColor(Qt::blue);
    QColor err_color = QColor(Qt::red);
    QColor war_color = QColor(Qt::yellow);

private:
public:
    reporter(network_statistic *stat, bool is_debug, QWidget *parent = nullptr) : QTextEdit(parent)
    {
        this->stat = stat;
        this->is_debug = is_debug;
        setReadOnly(true);
        setLineWrapMode(QTextEdit::NoWrap);
        setAlignment(Qt::AlignTop);
    }

    void print_message(std::string message);
    void print_debug_message(std::string message);
    void print_warning(std::string message);
    void print_error(std::string message);
};
