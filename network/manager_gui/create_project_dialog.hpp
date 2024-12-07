#include <QtWidgets/QtWidgets>
#include <iostream>
#include <string>

#pragma once

class create_project_dialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *label;
    QLineEdit *res_name;
    QPushButton *create_button;
    QPushButton *close_button;

public:
    create_project_dialog(QObject *parent)
    {
        setWindowTitle("Crate Project");
        setFixedSize(500, 100);
        label = new QLabel("Project name: ", this);
        res_name = new QLineEdit(this);

        label->setBuddy(res_name);
        create_button = new QPushButton("Create", this);
        close_button = new QPushButton("Close", this);
        create_button->setDefault(true);
        create_button->setEnabled(false);

        connect(res_name, SIGNAL(textChanged(const QString &)), this, SLOT(enableSaveButton(const QString &)));
        connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
        connect(create_button, SIGNAL(clicked()), parent, SLOT(crete_project_click()));

        QHBoxLayout *topLeftLayout = new QHBoxLayout;
        topLeftLayout->addWidget(label);
        topLeftLayout->addWidget(res_name);

        QVBoxLayout *leftLayout = new QVBoxLayout;
        leftLayout->addLayout(topLeftLayout);
        leftLayout->addStretch(1);

        QVBoxLayout *rightLayout = new QVBoxLayout;
        rightLayout->addWidget(create_button);
        rightLayout->addWidget(close_button);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setMargin(11);
        mainLayout->setSpacing(6);
        mainLayout->addLayout(leftLayout);
        mainLayout->addLayout(rightLayout);
    }

    QString get_result_name()
    {
        return res_name->text();
    }

public slots:
    void enableSaveButton(const QString &text)
    {
        create_button->setEnabled(!text.isEmpty());
    }
};