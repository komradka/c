#include "wf_action_areaitem.hpp"
#include <iostream>
#include <vector>

class wf_action;

class work_area : public QTreeWidget
{
    Q_OBJECT

private:
    std::vector<wf_action_areaitem *> actions;

public:
    work_area(QWidget *parent) : QTreeWidget(parent)
    {
        setHeaderLabel("Work Area");
        setColumnCount(1);
        setStyleSheet("QTreeView::item { height: 32px;}");
        setDragEnabled(true);
        viewport()->setAcceptDrops(true);
        showDropIndicator();
        setDragDropMode(QTreeWidget::InternalMove);
    }

private:
    void add_item(wf_action *act);

public slots:
    void add_action(QTreeWidgetItem *item, int column);
};