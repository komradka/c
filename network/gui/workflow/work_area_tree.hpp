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
    work_area(QWidget *parent);

    Qt::DropActions supportedDropActions() const;

private:
    void add_item(wf_action *act);

public slots:
    void add_action(QTreeWidgetItem *item, int column);
};