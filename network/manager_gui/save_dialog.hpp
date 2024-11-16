#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsScene>
#include <iostream>
#include <string>

#pragma once

class save_dialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *label;
    QLineEdit *res_name;
    QPushButton *close_button;
    QPushButton *save_button;

public:
    save_dialog(int res_num, QObject *parent)
    {
        std::string title = "Results " + std::to_string(res_num);
        setWindowTitle(QString::fromStdString(title));

        setFixedSize(500, 100);

        label = new QLabel("Result name: ", this);
        res_name = new QLineEdit(this);
        label->setBuddy(res_name);
        save_button = new QPushButton("Save", this);
        close_button = new QPushButton("Close", this);
        save_button->setDefault(true);
        save_button->setEnabled(false);

        connect(res_name, SIGNAL(textChanged(const QString &)), this, SLOT(enableSaveButton(const QString &)));
        connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
        connect(save_button, SIGNAL(clicked()), parent, SLOT(save_project_click()));

        QHBoxLayout *topLeftLayout = new QHBoxLayout;
        topLeftLayout->addWidget(label);
        topLeftLayout->addWidget(res_name);

        QVBoxLayout *leftLayout = new QVBoxLayout;
        leftLayout->addLayout(topLeftLayout);
        leftLayout->addStretch(1);

        QVBoxLayout *rightLayout = new QVBoxLayout;
        rightLayout->addWidget(save_button);
        rightLayout->addWidget(close_button);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setMargin(11);
        mainLayout->setSpacing(6);
        mainLayout->addLayout(leftLayout);
        mainLayout->addLayout(rightLayout);
    }

    void resizeEvent(QResizeEvent *event) override
    {
        event->ignore();
    }

    QString get_result_name()
    {
        return res_name->text();
    }

public slots:
    void enableSaveButton(const QString &text)
    {
        save_button->setEnabled(!text.isEmpty());
    }
};