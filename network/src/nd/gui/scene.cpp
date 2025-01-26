#include "scene.hpp"

void Scene::draw_link(it *first, it *second)
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

#include "moc_scene.cpp"
