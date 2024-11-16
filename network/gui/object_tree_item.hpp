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
#include <map>

#include "../kernel/topology.hpp"
#include "graph_item.hpp"

#pragma once

class objects_tree;

class object_tree_item : public QTreeWidgetItem
{

private:
    bool is_root_item = false;
    std::string name;
    QGraphicsItem *graph_item;

public:
    object_tree_item(network_objects type, objects_tree *parent);

    object_tree_item(std::string name, network_objects type, QGraphicsItem *graph_item, objects_tree *parent);

    std::string get_name();

    vertex *get_object();
};