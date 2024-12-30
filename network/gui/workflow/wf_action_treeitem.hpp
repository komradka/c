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

#include "../../workflow/workflow.hpp"

#pragma once

class action_tree;

class wf_action_treeitem : public QTreeWidgetItem
{
private:
    wf_action_category category;
    bool is_root = false;
    std::string tree_name;
    wf_action *action = nullptr;
    int id;

public:
    wf_action_treeitem(wf_action_category category, action_tree *parent);
    wf_action_treeitem(wf_action *action, action_tree *parent);

    int get_id() { return id; }
    bool is_root_item() { return is_root; }
    wf_action *get_action() { return action; }
};