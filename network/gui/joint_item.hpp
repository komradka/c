#include "graph_item.hpp"
#include "../kernel/topology.hpp"

#pragma once

class joint_item : public it
{
    Q_OBJECT

public:
    joint_item(vertex *v, QObject *parent = 0, QWidget *gui_manager = 0) : it(network_objects::joint, v, parent, gui_manager)
    {
        td = new tree_dialog_by_enum(500, dialog);
        td->setFixedSize(500, 700);

        auto create = [&](auto i)
        {
            td->add_widget<network_object_param_desc<joint_data::joint_params[i]>>();
        };

        constexpr_for<0, joint_data::joint_fields>(create);

        auto init = [&](auto i)
        {
            td->set_widget_param<network_object_param_desc<joint_data::joint_params[i]>>(i, v->get_data()->get_param<joint_data::joint_params[i]>());
        };

        constexpr_for<0, joint_data::joint_fields>(init);

        is_valid = true;
    }

    ~joint_item()
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
        QImage image("gui/resource/joint.png", "png");

        painter->drawImage(el, image);
        painter->drawText(QRect(-50, -60, 50, 50), Qt::AlignCenter, QString::fromStdString(v->get_data()->get_name()));
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};