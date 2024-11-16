#include "graph_item.hpp"
#include "../kernel/topology.hpp"

#pragma once

class pipe_item : public it
{
    Q_OBJECT

public:
    pipe_item(vertex *v, QObject *parent = 0, QWidget *gui_manager = 0) : it(network_objects::pipe, v, parent, gui_manager)
    {
        td = new tree_dialog_by_enum(500, dialog);
        td->setFixedSize(500, 700);

        auto create = [&](auto i)
        {
            td->add_widget<network_object_param_desc<pipe_data::pipe_params[i]>>();
        };

        constexpr_for<0, pipe_data::pipe_fields>(create);

        auto init = [&](auto i)
        {
            td->set_widget_param<network_object_param_desc<pipe_data::pipe_params[i]>>(i, v->get_data()->get_param<pipe_data::pipe_params[i]>());
        };

        constexpr_for<0, pipe_data::pipe_fields>(init);

        is_valid = true;
    }

    ~pipe_item()
    {
    }

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);

        painter->drawRect(-30, -20, 60, 30);

        painter->drawText(QRect(-25, -25, 50, 50), Qt::AlignCenter, QString::fromStdString(v->get_data()->get_name()));

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};