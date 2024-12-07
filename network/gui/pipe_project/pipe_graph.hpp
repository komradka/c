#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>
#include "../../gui_utility/plot/qcustomplot.h"
#include "../../gui_utility/check_list_widget.hpp"
#include "../../kernel/pipe_holder/pipe_calculator.hpp"

#pragma once

class pipe_graph : public QDialog
{
    Q_OBJECT

private:
    QCustomPlot *plot;
    list_by_enum *q_list;
    QLineEdit *pos_line;

    std::vector<pipe_param> *params = nullptr;
    double l;

public:
    pipe_graph(QWidget *parent = nullptr) : QDialog(parent)
    {
        resize(1200, 700);
        setMinimumWidth(500);
        setMinimumHeight(500);
        plot = new QCustomPlot(this);
        q_list = new list_by_enum(this);
        q_list->fill<pipe_param_type>();

        connect(q_list, SIGNAL(itemSelectionChanged()), this, SLOT(paint()));

        pos_line = new QLineEdit(this);
        pos_line->setReadOnly(true);
        connect(plot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(slotMouseMove(QMouseEvent *)));
    }

    void init(std::vector<pipe_param> *params, double pipe_length)
    {
        this->params = params;
        this->l = pipe_length;
    }

public slots:
    void paint()
    {
        if (params == nullptr)
            return;

        plot->clearItems();
        plot->rescaleAxes();

        auto j = get_current_item_index();

        plot->addGraph(plot->xAxis, plot->yAxis);
        plot->xAxis->setLabel("Length");
        plot->yAxis->setLabel(QString::fromStdString(get_name_for_gui((pipe_param_type)j)));

        plot->xAxis->setRange(0 - std::min(10., 0.01 * (l + 1)), l + std::min(10., 0.01 * (l + 1)));

        QVector<double> res(params->size());
        QVector<double> lv(params->size());

        double seg_l = l / params->size();
        for (unsigned int i = 0; i < params->size(); i++)
        {
            res[i] = (*params)[i].get_param((pipe_param_type)j);
            lv[i] = i * seg_l;
        }

        auto max = std::max_element(res.begin(), res.end());
        auto min = std::min_element(res.begin(), res.end());

        plot->yAxis->setRange(*min, *max);
        plot->graph(0)->setData(lv, res);
        plot->replot();

        res.clear();
        lv.clear();
    }

    void slotMouseMove(QMouseEvent *event)
    {
        double x = plot->xAxis->pixelToCoord(event->pos().x());
        double y = plot->yAxis->pixelToCoord(event->pos().y());

        QString coords = "(" + QString::number(x) + ";" + QString::number(y) + ")";

        pos_line->setText(coords);
    }

public:
    int get_current_item_index()
    {
        for (int i = 0; i < q_list->count(); i++)
        {
            QListWidgetItem *item = q_list->item(i);
            if (item->isSelected())
                return i;
        }

        return 0;
    }

    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        plot->setGeometry(0, 0, width() * 0.7, height() - 20);
        q_list->setGeometry(width() * 0.7, 0, width() * 0.3, height() - 20);
        pos_line->setGeometry(0, height() - 20, 100, 20);
    }
};