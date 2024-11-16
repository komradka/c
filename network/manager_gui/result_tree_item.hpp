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
#include <string>
#include <vector>

#include "result_info_storage.hpp"

#pragma once

class result_item : public QTreeWidgetItem
{
public:
    result_info result;

public:
    result_item(result_info result, QTreeWidget *parent) : QTreeWidgetItem(parent)
    {
        this->result = result;
        setText(0, QString::fromStdString(result.res_name));
    }
};