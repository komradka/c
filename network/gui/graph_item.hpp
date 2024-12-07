#include "object_dialog.hpp"
#include "../kernel/topology.hpp"
#include "../kernel/objects_type.hpp"
#include "../gui_utility/tree_dialog_by_enum.hpp"
#include "link_item.hpp"

#pragma once

class it : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    vertex *v;

    QList<link_item *> links;

    object_dialog *dialog;

    network_objects obj_type;

    tree_dialog_by_enum *td;

    bool is_valid = false;
    bool is_clicked = false;

public:
    it(network_objects _type = network_objects::unknown, vertex *v = 0, QObject *parent = 0, QWidget *gui_manager = 0) : QObject(parent), QGraphicsItem()
    {
        connect(this, SIGNAL(updated()), gui_manager, SLOT(object_updated()));

        dialog = new object_dialog(this);
        dialog->setFixedSize(500, 700);
        dialog->setWindowTitle(QString::fromStdString(get_name_for_gui(_type)));

        obj_type = _type;

        setData(1, "it");
        setZValue(1);

        this->v = v;
    }
    ~it()
    {
    }

    enum
    {
        Type = UserType + 1
    };

    int type() const
    {
        return Type;
    }

    void delete_link(QGraphicsLineItem *link)
    {
        auto iter = std::find(links.begin(), links.end(), link);
        if (iter != links.end())
        {
            links.erase(iter);
        }
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

        for (auto link : links)
        {
            QPointF linkP2 = link->line().p2();
            QLineF new_line = QLineF(itemPos, linkP2);
            link->redrow(new_line);
        }
    }

    bool check_links(link_direction new_link_dir, QLineF rhs)
    {
        if (!v->add_link_verification(new_link_dir))
            return false;

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
    void need_update()
    {
        if (!is_valid)
            return;

        switch (obj_type)
        {
        case network_objects::sink:
        {
            auto _sink_data = dynamic_cast<sink_data *>(v->get_data());
            auto init_sink = [&](auto i)
            {
                _sink_data->params[sink_data::sink_params[i]] = td->get_widget_param<network_object_param_desc<sink_data::sink_params[i]>>(i);
            };
            constexpr_for<0, sink_data::sink_fields>(init_sink);
            break;
        }
        case network_objects::source:
        {
            auto _source_data = dynamic_cast<source_data *>(v->get_data());
            auto init_source = [&](auto i)
            {
                _source_data->params[source_data::source_params[i]] = td->get_widget_param<network_object_param_desc<source_data::source_params[i]>>(i);
            };
            constexpr_for<0, source_data::source_fields>(init_source);
            break;
        }
        case network_objects::pipe:
        {
            auto _pipe_data = dynamic_cast<pipe_data *>(v->get_data());
            auto init_pipe = [&](auto i)
            {
                _pipe_data->params[pipe_data::pipe_params[i]] = td->get_widget_param<network_object_param_desc<pipe_data::pipe_params[i]>>(i);
            };
            constexpr_for<0, pipe_data::pipe_fields>(init_pipe);
            break;
        }
        case network_objects::joint:
        {
            auto _joint_data = dynamic_cast<joint_data *>(v->get_data());
            auto init_joint = [&](auto i)
            {
                _joint_data->params[joint_data::joint_params[i]] = td->get_widget_param<network_object_param_desc<joint_data::joint_params[i]>>(i);
            };
            constexpr_for<0, joint_data::joint_fields>(init_joint);
            break;
        }
        case network_objects::COUNT:
        case network_objects::link:
        case network_objects::unknown:
            return;
        }

        if (!v->get_data()->get_name().empty())
        {
            std::replace(v->get_data()->get_name().begin(), v->get_data()->get_name().end(), ' ', '_');
        }

        emit updated();
        this->update();
    }

signals:
    void updated();
};