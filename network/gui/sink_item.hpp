#include "graph_item.hpp"
#include "../kernel/topology.hpp"

#pragma once

class sink_item : public it
{
    Q_OBJECT

public:
    sink_item(vertex *v, QObject *parent = 0, QWidget *gui_manager = 0) : it(network_objects::sink, v, parent, gui_manager)
    {
        td = new tree_dialog_by_enum(500, dialog);
        td->setFixedSize(500, 700);

        auto create = [&](auto i)
        {
            td->add_widget<network_object_param_desc<sink_data::sink_params[i]>>();
        };

        constexpr_for<0, sink_data::sink_fields>(create);

        auto init = [&](auto i)
        {
            td->set_widget_param<network_object_param_desc<sink_data::sink_params[i]>>(i, v->get_data()->get_param<sink_data::sink_params[i]>());
        };

        constexpr_for<0, sink_data::sink_fields>(init);

        is_valid = true;
    }

    ~sink_item()
    {
    }

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        if (is_clicked)
        {
            painter->setPen(Qt::darkYellow);
            painter->drawRect(-30, -30, 60, 60);
        }

        QRectF el = QRectF(-30, -30, 60, 60);
        QImage image("gui/resource/sink.png", "png");

        painter->drawImage(el, image);
        painter->drawText(QRect(-50, -60, 50, 50), Qt::AlignCenter, QString::fromStdString(v->get_data()->get_name()));
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};