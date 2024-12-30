#include "work_area_tree.hpp"
#include "wf_action_tree.hpp"
#include <iostream>

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

    // private:
    //     void fill_wf_action()
    //     {

    //     }
};