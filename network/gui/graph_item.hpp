#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsScene>
#include <iostream>

#include "../kernel/topology.hpp"

#pragma once

class it : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    std::string name;

    vertex *v;

    QList<QGraphicsLineItem *> links;

public:
    it(QObject *parent = 0) : QObject(parent), QGraphicsItem()
    {
        setData(1, "it");
        setZValue(1);
    }
    ~it()
    {
    }

    void set_object(vertex *v)
    {
        this->v = v;
        this->name = v->get_data()->get_name_for_gui();
    }

    enum
    {
        Type = UserType + 1
    };

    int type() const
    {
        return Type;
    }

signals:

public:
    QRectF boundingRect() const
    {
        return QRectF(-30, -30, 60, 60);
    }

    void adjust_links()
    {
        QPointF dp = this->boundingRect().center();
        QPointF itemPos = this->scenePos() + dp;

        for (auto link : links)
        {
            QPointF linkP1 = link->line().p1();
            QPointF linkP2 = link->line().p2();

            if (linkP2 == itemPos)
            {
                link->setLine(QLineF(linkP2, linkP1));
            }
            else
            {
                link->setLine(QLineF(linkP1, linkP2));
            }
        }
    }

    void redraw_link()
    {
        QPointF dp = this->boundingRect().center();
        QPointF itemPos = this->scenePos() + dp;

        for(auto link : links)
        {
            QPointF linkP2 = link->line().p2();
            link->setLine(QLineF(itemPos, linkP2));
        }
    }

    bool check_links(QLineF rhs)
    {
        for (auto link : links)
        {
            QLineF normalLine = link->line();
            QPointF p1 = normalLine.p1();
            QPointF p2 = normalLine.p2();
            QLineF invertedLine = QLineF(p2, p1);

            if (normalLine == rhs || invertedLine == rhs)
                return false;
        }
        return true;
    }

private:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::green);

        painter->drawEllipse(-30, -30, 60, 60);
        painter->drawText(QRect(-25, -25, 50, 50), Qt::AlignCenter, "AAA");

        Q_UNUSED(option);
        Q_UNUSED(widget);
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        this->setPos(mapToScene(event->pos()));
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        Q_UNUSED(event);
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        this->setCursor(QCursor(Qt::ArrowCursor));
        Q_UNUSED(event);
    }

public slots:
};