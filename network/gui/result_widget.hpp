#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

#include <iostream>
#include <vector>

#include "object_tree.hpp"
#include "../kernel/phys_q.hpp"

#pragma once

class result_widget : public QWidget
{
    Q_OBJECT

public:
    QListWidget *quantity_list;
    QTableWidget *table;
    
    objects_tree *objects_list; // not owner

    std::map<object_id, phys_q> object_results;

public:
    result_widget(objects_tree *ol)
    {
        objects_list = ol;
        quantity_list = new QListWidget(this);

        for (int i = 0; i < (int)phys_q_type::COUNT; i++)
        {
            QListWidgetItem *item = new QListWidgetItem(quantity_list);
            item->setText(QString::fromStdString(get_name_for_gui(phys_q_type(i))));
            item->setCheckState(Qt::Unchecked);
        }

        connect(objects_list, SIGNAL(itemSelectionChanged()), this, SLOT(object_tree_selection_changed()));
        connect(quantity_list, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(list_item_clicked(QListWidgetItem *)));

        table = new QTableWidget(this);
    }

    void reprint_table();
    std::vector<int> get_checked_items();

public slots:
    void list_item_clicked(QListWidgetItem *item)
    {
        Q_UNUSED(item);
        reprint_table();
    }

    void object_tree_selection_changed()
    {
        reprint_table();
    }

private:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        table->setGeometry(0, 0, 0.8 * width(), height());
        quantity_list->setGeometry(0.8 * width(), 0, 0.2 * width(), height());
    }
};
