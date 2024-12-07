#include "fluid_experiment_dialog.hpp"
#include "../kernel/fluid/experiment/fluid_experiment.hpp"

#include <iostream>

experiment_dialog::experiment_dialog(QWidget *parent) : QDialog(parent)
{
    p = parent;
    setFixedSize(300, 186);
    setWindowTitle("Experiment");

    QLabel *rate_label = new QLabel("Water Volume Rate SC", this);
    rate_label->setGeometry(1, 0, width() / 2, 30);
    rate_line = new double_textEdit(this);
    rate_line->setGeometry(width() / 2 + 1, 0, width() / 2, 30);
    rate_label->setBuddy(rate_line);

    QLabel *pressure_label = new QLabel("Pressure", this);
    pressure_label->setGeometry(1, 31, width() / 2, 30);
    pressure_line = new double_textEdit(this);
    pressure_line->setGeometry(width() / 2 + 1, 31, width() / 2, 30);
    pressure_label->setBuddy(pressure_line);

    QLabel *temp_label = new QLabel("Temperature", this);
    temp_label->setGeometry(1, 62, width() / 2, 30);
    temperature_line = new double_textEdit(this);
    temperature_line->setGeometry(width() / 2 + 1, 62, width() / 2, 30);
    temp_label->setBuddy(temperature_line);

    QLabel *results_label = new QLabel("Result Type", this);
    results_label->setGeometry(1, 93, width() / 2, 30);
    result_type = new enum_widget(this);
    result_type->fill_widget<water_calculated_props>();
    result_type->setGeometry(width() / 2 + 1, 93, width() / 2, 30);
    results_label->setBuddy(result_type);

    calc_button = new QPushButton("Calculate", this);
    calc_button->setGeometry(1, 155, width() - 2, 30);

    QLabel *result = new QLabel("Result", this);
    result->setGeometry(1, 124, width() / 2, 30);
    result_line = new QLineEdit(this);
    result_line->setGeometry(width() / 2 + 1, 124, width() / 2, 30);
    result->setBuddy(result_line);
    result_line->setReadOnly(true);

    connect(calc_button, SIGNAL(clicked()), this, SLOT(start_experiment()));
}

void experiment_dialog::start_experiment()
{
    result_line->clear();
    auto rate = rate_line->get_num();
    auto pressure = pressure_line->get_num();
    auto temp = temperature_line->get_num();

    if (!rate || !pressure || !temp)
    {
        result_line->setText("Fill all field");
        return;
    }

    auto res_type = (water_calculated_props)result_type->currentIndex();

    fluid_experiment *experiment = new fluid_experiment(rate.value(), pressure.value(), temp.value(), res_type, p);

    double res;

    error ret = experiment->start_experiment(res);

    if (!ret.is_ok())
    {
        result_line->setText(QString::fromStdString(ret));
        return;
    }

    result_line->setText(QString::number(res));

    delete experiment;
}