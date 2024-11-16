#include "graph_item.hpp"

class link_item : public it
{
public:
    QPoint start;
    QPoint end;

public:
    link_item(QObject *parent = 0) : it(network_objects::link, 0, parent)
    {
    }

    ~link_item()
    {
    }

    void set_coords(QPoint f, QPoint s)
    {
        start = f;
        end = s;
    }

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::blue);

        painter->drawLine(start, end);

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
};
