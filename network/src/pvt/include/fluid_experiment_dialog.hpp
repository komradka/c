#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

#include "../../common_gui/include/number_textEditer.hpp"
#include "../../common_gui/include/enum_widget.hpp"

#pragma once

class fluid_experiment;

class experiment_dialog : public QDialog
{
    Q_OBJECT

private:
    double_textEdit *rate_line;
    double_textEdit *pressure_line;
    double_textEdit *temperature_line;
    QLineEdit *result_line;
    enum_widget *result_type;
    QPushButton *calc_button;

    QWidget *p;

private:
    fluid_experiment *experiment;

public:
    experiment_dialog(QWidget *parent);

public:
    Q_SLOT void start_experiment();
};
