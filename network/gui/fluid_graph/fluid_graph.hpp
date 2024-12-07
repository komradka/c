#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

#include <string>

#include "../../gui_utility/plot/qcustomplot.h"
#include "../../gui_utility/enum_widget.hpp"
#include "../../gui_utility/number_textEditer.hpp"

#pragma once

enum class x_axis_param
{
    pressure,
    temperature,
    volume_rate_sc,

    COUNT
};

std::string get_name_for_gui(x_axis_param e);

class fluid_plot : public QDialog
{
    Q_OBJECT

private:
    QWidget *p;
    QCPItemTracer *tracer;

    QCustomPlot *plot;
    QLineEdit *pos_line;

    QPushButton *replot_button;

    QLabel *y_axis;
    enum_widget *y_axis_p;
    QLabel *x_axis;
    enum_widget *x_axis_p;

    QLabel *diapason;
    double_textEdit *left_boundary;
    double_textEdit *right_boundary;
    QLabel *point_count;
    int_textEdit *point_count_line;

    QLabel *param1;
    double_textEdit *param1_line;
    QLabel *param2;
    double_textEdit *param2_line;

public:
    fluid_plot(QWidget *parent);

private:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void slotMouseMove(QMouseEvent *event);

    void change_x_param(int i);

    void paint_plot();
};