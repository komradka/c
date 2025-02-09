#include "workflow_dialog.hpp"

workflow_dialog::workflow_dialog(workflow *wf)
{
    setWindowTitle("Workflow");

    this->wf = wf;

    area = new work_area(this);
    action = new action_tree(wf, this);
    start_calculation = new QPushButton("Start", this);

    connect(action, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), area, SLOT(add_action(QTreeWidgetItem *, int)));
    connect(start_calculation, SIGNAL(clicked()), this, SLOT(start_holder()));
}

void workflow_dialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    area->setGeometry(0, 0, width() * 2 / 3, height());
    action->setGeometry(width() * 2 / 3, 0, width() / 3, height() - 30);
    start_calculation->setGeometry(width() * 2 / 3, height() - 30, width() / 3, 30);
}

void workflow_dialog::start_holder()
{
    start_calculation_signal();
}

#include "moc_workflow_dialog.cpp"
