
#include "result_tree_item.hpp"

#pragma once

class load_dialog : public QDialog
{
    Q_OBJECT

public:
    QTreeWidget *result_tree;
    int res_num = 0;

private:
    QPushButton *close_button;
    QPushButton *load_button;

public:
    load_dialog(std::vector<result_info> &results, QObject *parent)
    {
        setWindowTitle("Load Results ...");
        setFixedSize(1000, 500);

        result_tree = new QTreeWidget(this);
        result_tree->setColumnCount(1);
        result_tree->setHeaderLabel("Results: ");

        // result_tree->setGeometry(0, 0, this->width() / 3, this->height() - 1);

        close_button = new QPushButton("Close", this);
        load_button = new QPushButton("Load", this);

        load_button->setDefault(true);
        load_button->setEnabled(false);

        connect(result_tree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(enable_load_button(QTreeWidgetItem *, int)));
        connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
        connect(load_button, SIGNAL(clicked()), parent, SLOT(change_results()));

        res_num = results.size();

        QVBoxLayout *leftLayout = new QVBoxLayout;
        leftLayout->addWidget(result_tree);
        leftLayout->addStretch(1);

        QVBoxLayout *rightLayout = new QVBoxLayout;
        rightLayout->addWidget(load_button);
        rightLayout->addWidget(close_button);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setMargin(11);
        mainLayout->setSpacing(6);
        mainLayout->addLayout(leftLayout);
        mainLayout->addLayout(rightLayout);

        for (int i = 0; i < (int)results.size(); i++)
        {
            result_item *res_item = new result_item(results[i], result_tree);
            result_tree->addTopLevelItem(res_item);
        }
    }

private slots:
    void enable_load_button(QTreeWidgetItem *item, int col)
    {
        Q_UNUSED(col);
        load_button->setEnabled(item != nullptr);
    }
};