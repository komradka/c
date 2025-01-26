#include "object_tree.hpp"

void objects_tree::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        object_tree_item *si = dynamic_cast<object_tree_item *>(selectedItems()[0]);
        int id = si->get_id();

        delete selectedItems()[0];

        if (si->is_link())
            emit delete_link(id);
        else
            emit delete_object(id);
    }
}

#include "moc_object_tree.cpp"
