#include "wf_action_areaitem.hpp"
#include <iostream>
#include <vector>
#include <map>

class wf_action;
#pragma once
class work_area : public QTreeWidget
{
    Q_OBJECT

private:
    std::vector<wf_action_areaitem *> actions;

public:
    int action_count = 0;

public:
    work_area(QWidget *parent);

    Qt::DropActions supportedDropActions() const;
    void dropEvent(QDropEvent *event);

private:
    void add_item(wf_action *act);
    void update_widgets();

public:
    Q_SLOT void add_action(QTreeWidgetItem *item, int column);
};
