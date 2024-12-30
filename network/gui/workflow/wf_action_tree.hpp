#include "wf_action_treeitem.hpp"
#include "../../utility/utility.hpp"

class action_tree : public QTreeWidget
{
    Q_OBJECT
private:
    std::map<wf_action_category, QTreeWidgetItem *> roots;

public:
    action_tree(workflow *wf, QWidget *parent) : QTreeWidget(parent)
    {
        setHeaderLabel("Actions");

        fill_action(wf);
    }

    friend class wf_action_treeitem;

private:
    void fill_action(workflow *wf);

    void fill_root_elements();

    void fill_sub_element(workflow *wf);
};