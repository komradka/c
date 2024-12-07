#include "../../kernel/pipe_holder/pipe_calculator.hpp"
#include "../../gui_utility/tree_dialog_by_enum.hpp"
#include "../../gui_utility/number_textEditer.hpp"
#include "pipe_graph.hpp"

#pragma once

class pipe_project : public QWidget
{
    Q_OBJECT

private:
    tree_dialog_by_enum *td;
    QPushButton *calc_button;
    QPushButton *graph_button;
    pipe_graph *graph;

    QLabel *pressure_drop;
    QLabel *temperature_drop;

    double_textEdit *pressure_drop_l;
    double_textEdit *temperature_drop_l;

    pipe_calculator *calculator = nullptr;
    pipe_data data;

    fluid_widget *fw;

public:
    pipe_project(fluid_widget *fw)
    {
        this->fw = fw;

        td = new tree_dialog_by_enum(500, this);
        auto create = [&](auto i)
        {
            td->add_widget<network_object_param_desc<pipe_data::pipe_params[i]>>();
        };

        constexpr_for<2, pipe_data::pipe_fields>(create);

        calc_button = new QPushButton("Calculate", this);
        graph_button = new QPushButton("Visualize", this);
        connect(graph_button, SIGNAL(clicked()), this, SLOT(visualize()));
        connect(calc_button, SIGNAL(clicked()), this, SLOT(calculate()));

        pressure_drop = new QLabel("Pressure drop", this);
        pressure_drop_l = new double_textEdit(this);
        pressure_drop_l->setText("0");
        pressure_drop_l->setReadOnly(true);

        temperature_drop = new QLabel("Temperature drop", this);
        temperature_drop_l = new double_textEdit(this);
        temperature_drop_l->setText("0");
        temperature_drop_l->setReadOnly(true);

        graph = new pipe_graph();
    }

private:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        td->setGeometry(0, 0, width() * 0.7, height());
        calc_button->setGeometry(width() * 0.7, 0, width() * 0.3, 30);
        graph_button->setGeometry(width() * 0.7, 31, width() * 0.3, 30);

        pressure_drop->setGeometry(width() * 0.7, 62, width() * 0.15, 30);
        pressure_drop_l->setGeometry(width() * 0.85, 62, width() * 0.15, 30);

        temperature_drop->setGeometry(width() * 0.7, 93, width() * 0.15, 30);
        temperature_drop_l->setGeometry(width() * 0.85, 93, width() * 0.15, 30);
    }

public slots:
    void visualize()
    {
        if (calculator == nullptr)
            return;
        graph->show();
        graph->init(&calculator->get_report_params(), data.get_param<network_object_param::pipe_len>().value());
    }

    void calculate()
    {
        delete calculator;

        auto init_pipe = [&](auto i)
        {
            data.params[pipe_data::pipe_params[i]] = td->get_widget_param<network_object_param_desc<pipe_data::pipe_params[i]>>(i - 2);
        };

        constexpr_for<2, pipe_data::pipe_fields>(init_pipe);

        calculator = new pipe_calculator(&data, fw);

        double dp;
        double dt;

        error ret = calculator->compute_pressure_and_temperature_drop(10000, 100. - constant::ABS_ZERO_IN_CELSIUS, 5, dp, dt);
        if(!ret.is_ok())
        {
            pressure_drop_l->setText("Did not converge");
            return;
        }

        pressure_drop_l->setText(QString::number(dp));
        temperature_drop_l->setText(QString::number(dt));
    }
};