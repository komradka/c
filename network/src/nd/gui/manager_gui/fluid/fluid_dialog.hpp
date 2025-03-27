#include "pvt/include/pvt_manager.hpp"
#include "fluid_list.hpp"

#pragma once

class nd_main_window;

class fluid_dialog : public QDialog
{
    Q_OBJECT
private:
    fluid_list *list;
    nd_main_window *nd_window;

public:
    fluid_dialog(nd_main_window *nd_window)
    {
        setWindowTitle("Fluid Settings");
        setFixedSize(400, 700);

        this->nd_window = nd_window;

        list = new fluid_list(this);

        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(show_menu(const QPoint &)));
    }

    void resizeEvent(QResizeEvent *event)
    {
        list->setGeometry(0, 0, 400, 700);
    }

public:
    Q_SLOT void show_menu(const QPoint &pos)
    {
        QMenu cMenu("Actions", this);
        QAction add_act("Add PVT Project", this);
        connect(&add_act, SIGNAL(triggered()), this, SLOT(add_fluid()));
        cMenu.addAction(&add_act);
        cMenu.exec(mapToGlobal(pos));
    }

    Q_SLOT void add_fluid();

    Q_SLOT void export_fluid(std::string name, std::string path);

    Q_SLOT void change_fluid();
};
