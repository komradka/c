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

#include <string>
#include "wf_action_treeitem.hpp"

#include <iostream>

#pragma once

class wf_action_areaitem : public QTreeWidgetItem
{
private:
    std::string name;
    int id;
    wf_action_utility utility;

public:
    wf_action_areaitem(wf_action *act, QTreeWidget *parent);

    int get_id() { return id; }
};