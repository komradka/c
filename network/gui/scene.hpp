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

#include "source_item.hpp"
#include "sink_item.hpp"
#include "pipe_item.hpp"
#include "joint_item.hpp"

#pragma once

class Scene : public QGraphicsScene
{
    Q_OBJECT

private:
    std::map<object_id, it *> *items;
    std::map<link_id, link_item *> *links;

private:
    it *pressed_item = nullptr;
    it *target_item = nullptr;

    it *marker;
    QGraphicsLineItem *markerLine;

private:
    bool is_pressed = false;
    bool is_line_drawing = false;
    bool is_area_drawing = false;

    QPointF mouse_shift;

public:
    bool link_add_flag = false;

public:
    Scene(QObject *parent) : QGraphicsScene(parent)
    {
        connect(this, SIGNAL(link_created(vertex *, vertex *, link_item *)), parent, SLOT(add_link(vertex *, vertex *, link_item *)));

        marker = new it();
        marker->setData(1, "marker");
        marker->hide();
        addItem(marker);

        markerLine = new QGraphicsLineItem;
        markerLine->hide();
        addItem(markerLine);
    }

    void set_items_pointer(std::map<object_id, it *> *i, std::map<link_id, link_item *> *l)
    {
        items = i;
        links = l;
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        pressed_item = static_cast<it *>(this->itemAt(event->scenePos().x(),
                                                      event->scenePos().y(),
                                                      QTransform()));

        if (pressed_item->data(1) != "it")
            pressed_item = nullptr;

        if (pressed_item && link_add_flag)
        {
            is_pressed = true;
            is_line_drawing = true;
            mouse_shift = pressed_item->pos() - event->scenePos();
        }
        else if (pressed_item && !link_add_flag)
        {
            is_pressed = true;
            mouse_shift = pressed_item->pos() - event->scenePos();
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (pressed_item && is_pressed && !is_line_drawing)
        {
            if (!pressed_item->links.empty())
            {
                pressed_item->adjust_links();
                pressed_item->setPos(event->scenePos() + mouse_shift);
                pressed_item->redraw_link();
            }

            pressed_item->setPos(event->scenePos() + mouse_shift);
        }
        else if (pressed_item && is_pressed && is_line_drawing)
        {
            marker->setPos(event->scenePos() + mouse_shift);
            // marker->show();

            QPointF dp1 = pressed_item->boundingRect().center();
            QPointF dp2 = marker->boundingRect().center();

            QPointF p1 = pressed_item->scenePos() + dp1;
            QPointF p2 = marker->scenePos() + dp2;

            if (marker->contains(p1) == false)
            {
                markerLine->setLine(QLineF(p1, p2));
                markerLine->show();
            }

            if (marker->collidingItems().at(1) && marker->collidingItems().at(1)->data(1) == "it" &&
                marker->collidingItems().at(1) != pressed_item && is_line_drawing)
            {
                target_item = static_cast<it *>(marker->collidingItems().at(1));
            }
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        Q_UNUSED(event);
        if (is_line_drawing && target_item != nullptr && pressed_item != nullptr)
        {
            is_pressed = false;
            is_line_drawing = false;
            marker->hide();
            markerLine->hide();
            link_add_flag = false;

            draw_link(pressed_item, target_item);
        }
        is_pressed = false;
        is_line_drawing = false;
        marker->hide();
        markerLine->hide();
        target_item = nullptr;
        pressed_item = nullptr;
        link_add_flag = false;
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        auto pi = static_cast<it *>(this->itemAt(event->scenePos().x(),
                                                 event->scenePos().y(),
                                                 QTransform()));

        if (pi->data(1) == "it")
        {
            pi->dialog->show();
        }
    }

    void draw_link(it *first, it *second)
    {
        QPointF first_p = first->scenePos();
        QPointF second_p = second->scenePos();

        QLineF ql = QLineF(first_p, second_p);

        if (first->check_links(link_direction::outlet, ql) && second->check_links(link_direction::inlet, ql))
        {
            link_item *link = new link_item(ql);
            // QGraphicsLineItem *line = new QGraphicsLineItem;
            // line->setLine(ql);
            addItem(link);
            first->links.append(link);
            second->links.append(link);
            emit link_created(first->v, second->v, link);
        }
    }

    void delete_object(it *obj, std::vector<it *> connected_objs)
    {
        for (auto line : obj->links)
        {
            for (auto i : connected_objs)
            {
                i->delete_link(line);
            }
            delete line;
        }

        delete obj;
    }

    void delete_link(QGraphicsLineItem *link, std::vector<it *> connected_objects)
    {
        for (it *obj : connected_objects)
        {
            obj->delete_link(link);
        }

        delete link;
    }

signals:
    void link_created(vertex *f, vertex *s, link_item *link);
};
