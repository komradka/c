#include "fluid_list_item.hpp"

class fluid_list : public QListWidget
{
    Q_OBJECT
public:
    fluid_list_item *selected_fluid = nullptr;

public:
    fluid_list(QWidget *parent) : QListWidget(parent)
    {
        setStyleSheet("QListWidget::item { height: 32px; border: 1px solid black;}");
        connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(on_dbclick_play(QListWidgetItem *)));
        connect(this, SIGNAL(selected_fluid_changed()), parent, SLOT(change_fluid()));
    }

    void add_fluid(int id, std::string name)
    {
        fluid_list_item *it = new fluid_list_item(name, id);
        it->setFlags(it->flags() & ~Qt::ItemIsSelectable);
        addItem(it);
    }

    Q_SLOT void on_dbclick_play(QListWidgetItem *item)
    {
        if (selected_fluid)
            selected_fluid->setForeground(Qt::black);

        item->setForeground(Qt::red);
        selected_fluid = dynamic_cast<fluid_list_item *>(item);
        emit selected_fluid_changed();
    }

    Q_SIGNAL void selected_fluid_changed();
};