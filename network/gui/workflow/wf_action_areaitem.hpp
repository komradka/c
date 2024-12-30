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

#pragma once

class wf_action_areaitem : public QTreeWidgetItem
{
private:
    std::string name;
    int id;

public:
    wf_action_areaitem(std::string name, int id, QTreeWidget *parent) : QTreeWidgetItem(parent)
    {
        this->name = name;
        this->id = id;
        parent->addTopLevelItem(this);
        setText(0, QString::fromStdString(name));
    }
};