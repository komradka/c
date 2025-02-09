#include "work_area_tree.hpp"
#include "wf_action_tree.hpp"
#include <iostream>
#pragma once
class workflow_dialog : public QDialog
{
    Q_OBJECT
private:
    QPushButton *start_calculation;
    work_area *area;
    action_tree *action;

    QList<QTreeWidgetItem *> list;

    workflow *wf;

public:
    QSize minimumSizeHint() const
    {
        return QSize(600, 100);
    }

    QSize sizeHint() const
    {
        return QSize(1200, 1000);
    }

    workflow_dialog(workflow *wf);

    void resizeEvent(QResizeEvent *event) override;

    work_area *get_work_area() { return area; }

    // Signals
    Signal<> start_calculation_signal;

public:
    Q_SLOT void start_holder();
};
