#include "workflow_dialog.hpp"

workflow_dialog::workflow_dialog(workflow *wf)
{
    setWindowTitle("Workflow");

    this->wf = wf;

    area = new work_area(this);
    action = new action_tree(wf, this);

    connect(action, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), area, SLOT(add_action(QTreeWidgetItem *, int)));
}

void workflow_dialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    area->setGeometry(0, 0, width() * 2 / 3, height());
    action->setGeometry(width() * 2 / 3, 0, width() / 3, height());
    // start_calculation->setGeometry(width() - 70, height() - 30, 65, 20);
}