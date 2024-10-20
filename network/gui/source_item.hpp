#include "graph_item.hpp"
#include "../kernel/topology.hpp"

#pragma once

class source_item : public it
{
    Q_OBJECT

public:
    source_item(QObject *parent = 0) : it(parent)
    {
    }

    ~source_item()
    {
    }

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::red);

        painter->drawEllipse(-30, -30, 60, 60);

        painter->drawText(QRect(-25, -25, 50, 50), Qt::AlignCenter, QString::fromStdString(name));
        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};