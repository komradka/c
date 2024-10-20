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

#pragma once

using namespace std;

class reporter : public QTextEdit
{
    Q_OBJECT
private:
    QScrollBar *sb;
    QColor msg_color = QColor(Qt::blue);
    QColor err_color = QColor(Qt::red);
    QColor war_color = QColor(Qt::darkYellow);

public:
    reporter(QWidget *parent = nullptr) : QTextEdit(parent)
    {
        setReadOnly(true);
        setLineWrapMode(QTextEdit::NoWrap);
        setAlignment(Qt::AlignTop);
    }

    void print_message(std::string message);
    void print_warning(std::string message);
    void print_error(std::string message);
};

