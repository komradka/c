#include "fluid_graph.hpp"
#include "../../kernel/fluid/experiment/fluid_experiment.hpp"

#include <iostream>
#include <vector>

std::string get_name_for_gui(x_axis_param e)
{
    switch (e)
    {
    case x_axis_param::pressure:
        return "Pressure";
    case x_axis_param::temperature:
        return "Temperature";
    case x_axis_param::volume_rate_sc:
        return "Volume Rate SC";
    case x_axis_param::COUNT:
        return "";
    }
    return "";
}

std::string get_unit(x_axis_param e)
{
    switch (e)
    {
    case x_axis_param::pressure:
        return ", bar";
    case x_axis_param::temperature:
        return ", C";
    case x_axis_param::volume_rate_sc:
        return ", sm3";
    case x_axis_param::COUNT:
        return "";
    }
    return "";
}

fluid_plot::fluid_plot(QWidget *parent) : QDialog(parent)
{
    p = parent;
    resize(1200, 700);
    plot = new QCustomPlot(this);

    replot_button = new QPushButton("Paint", this);

    connect(replot_button, SIGNAL(clicked()), this, SLOT(paint_plot()));
    connect(plot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(slotMouseMove(QMouseEvent *)));

    pos_line = new QLineEdit(this);
    pos_line->setReadOnly(true);

    y_axis = new QLabel("Y Axis: ", this);
    y_axis_p = new enum_widget(this);
    y_axis_p->fill_widget<water_calculated_props>();

    x_axis = new QLabel("X Axis: ", this);
    x_axis_p = new enum_widget(this);
    x_axis_p->fill_widget<x_axis_param>();

    connect(x_axis_p, SIGNAL(currentIndexChanged(int)), this, SLOT(change_x_param(int)));

    diapason = new QLabel("Diapason: ", this);

    left_boundary = new double_textEdit(this);
    left_boundary->setText(QString::number(2));
    right_boundary = new double_textEdit(this);
    right_boundary->setText(QString::number(10));

    point_count = new QLabel("Point Amount", this);
    point_count_line = new int_textEdit(this);
    point_count_line->setText(QString::number(10));

    std::string param1_s = "Reference " + get_name_for_gui(x_axis_param::temperature) + get_unit(x_axis_param::temperature);
    param1 = new QLabel(QString::fromStdString(param1_s), this);
    std::string param2_s = "Reference " + get_name_for_gui(x_axis_param::volume_rate_sc) + get_unit(x_axis_param::volume_rate_sc);
    param2 = new QLabel(QString::fromStdString(param2_s), this);

    param1_line = new double_textEdit(this);
    param2_line = new double_textEdit(this);
}

void fluid_plot::resizeEvent(QResizeEvent *event)
{
    pos_line->setGeometry(0, height() - 20, 100, 20);
    plot->setGeometry(0, 0, width() * 0.7, height() - 20);

    replot_button->setGeometry(width() * 0.7, 0, width() * 0.3 - 1, 40);

    y_axis->setGeometry(width() * 0.7, 41, width() * 0.15, 30);
    y_axis_p->setGeometry(width() * 0.85, 41, width() * 0.15, 30);

    x_axis->setGeometry(width() * 0.7, 72, width() * 0.15, 30);
    x_axis_p->setGeometry(width() * 0.85, 72, width() * 0.15, 30);

    diapason->setGeometry(width() * 0.7, 103, width() * 0.15, 30);
    left_boundary->setGeometry(width() * 0.85, 103, width() * 0.15 / 2, 30);
    right_boundary->setGeometry(width() * 0.85 + width() * 0.15 / 2, 103, width() * 0.15 / 2, 30);

    point_count->setGeometry(width() * 0.7, 134, width() * 0.15, 30);
    point_count_line->setGeometry(width() * 0.85, 134, width() * 0.15, 30);

    param1->setGeometry(width() * 0.7, 165, width() * 0.15, 30);
    param2->setGeometry(width() * 0.7, 196, width() * 0.15, 30);

    param1_line->setGeometry(width() * 0.85, 165, width() * 0.15, 30);
    param2_line->setGeometry(width() * 0.85, 196, width() * 0.15, 30);
    resize(event->size());
}

void fluid_plot::slotMouseMove(QMouseEvent *event)
{
    double x = plot->xAxis->pixelToCoord(event->pos().x());
    double y = plot->yAxis->pixelToCoord(event->pos().y());

    QString coords = "(" + QString::number(x) + ";" + QString::number(y) + ")";

    pos_line->setText(coords);
}

void fluid_plot::change_x_param(int i)
{
    std::vector<int> p = {0, 1, 2};
    auto it = std::find(p.begin(), p.end(), i);
    p.erase(it);

    std::string param1_s = "Reference " + get_name_for_gui(x_axis_param(p[0])) + get_unit(x_axis_param(p[0]));
    std::string param2_s = "Reference " + get_name_for_gui(x_axis_param(p[1])) + get_unit(x_axis_param(p[1]));

    param1->setText(QString::fromStdString(param1_s));
    param2->setText(QString::fromStdString(param2_s));
}

double find_max(QVector<double> &v)
{
    double max = v[0];

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] > max)
            max = v[i];
    }

    return max;
}

double find_min(QVector<double> &v)
{
    double min = v[0];

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] < min)
            min = v[i];
    }

    return min;
}

void fluid_plot::paint_plot()
{
    plot->clearItems();
    plot->rescaleAxes();

    auto lb = left_boundary->get_num();
    auto rb = right_boundary->get_num();
    auto n = point_count_line->get_num();

    auto p1 = param1_line->get_num();
    auto p2 = param2_line->get_num();

    if (!lb || !rb || !n || !p1 || !p2)
    {
        return;
    }

    if (n.value() < 2)
        n = 2;
    if (n.value() > (rb.value() - lb.value())- 1)
        n = (rb.value() - lb.value()) - 1;

    plot->addGraph(plot->xAxis, plot->yAxis);
    plot->yAxis->setLabel(QString::fromStdString(get_name_for_gui((water_calculated_props)y_axis_p->currentIndex())));
    plot->xAxis->setLabel(QString::fromStdString(get_name_for_gui((x_axis_param)x_axis_p->currentIndex())));

    plot->xAxis->setRange(lb.value() - std::min(0.01, 0.01 * (lb.value() + 1)), rb.value() + std::min(0.01, 0.01 * (rb.value() + 1)));

    fluid_experiment *experiment = new fluid_experiment((water_calculated_props)y_axis_p->currentIndex(), p);

    QVector<double> y(n.value(), 0);
    QVector<double> x(n.value(), 0);

    double h = (rb.value() - lb.value()) / n.value();
    for (int i = 0; i <= n.value(); i++)
    {
        double p, t, v;
        int ind = x_axis_p->currentIndex();

        x[i] = lb.value() + i * h;

        if (ind == 0)
        {
            p = x[i];
            t = p1.value();
            v = p2.value();
        }
        else if (ind == 1)
        {
            p = p1.value();
            t = x[i];
            v = p2.value();
        }
        else
        {
            p = p1.value();
            t = p2.value();
            v = x[i];
        }

        experiment->start_experiment(v, p, t, y[i]);
    }

    double min = find_min(y);
    double max = find_max(y);

    plot->yAxis->setRange(min - std::min(0.01, 0.001 * (min + 1)), max + std::min(0.01, 0.001 * (min + 1)));

    plot->graph(0)->setData(x, y);

    plot->replot();

    delete experiment;
    x.clear();
    y.clear();
}