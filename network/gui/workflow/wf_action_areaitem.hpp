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
    QString storage;

public:
    int inner_id;
    QWidget *utility_widget;

public:
    wf_action_areaitem(wf_action *act, QTreeWidget *parent);

    int get_id() { return id; }

    wf_action_utility get_utility() { return utility; }

    void save_utility()
    {
        switch (utility)
        {
            case wf_action_utility::none:
            {
                return;
            }
            case wf_action_utility::string:
            {
                QLineEdit *line = dynamic_cast<QLineEdit *>(utility_widget);
                storage = line->text();
                // delete utility_widget;
                return;
            }
            case wf_action_utility::file:
            {
                return;
            }
            case wf_action_utility::COUNT:
            {
                return;
            }
        }
    }

    QWidget *restore_utility()
    {
        switch (utility)
        {
            case wf_action_utility::none:
            {
                return nullptr;
            }
            case wf_action_utility::string:
            {
                QLineEdit *line = new QLineEdit();
                line->setText(storage);
                utility_widget = line;
                return utility_widget;
            }
            case wf_action_utility::file:
            {
                return nullptr;
            }
            case wf_action_utility::COUNT:
            {
                return nullptr;
            }
        }

        return nullptr;
    }
};