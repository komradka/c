#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

#include <string>
#include <iostream>

#include "../managers/project_manager.hpp"

#pragma once

struct button_settings
{
    std::string name;
    std::string default_colour;
    std::string hover_color;
    int pos;
    project_type type;
};

class project_button : public QWidget
{
    Q_OBJECT
private:
    button_settings settings;
    QLabel *name_label = nullptr;
    QFont font = QFont("Comic Sans", 30, QFont::Bold);

    QPushButton *make_project_button = nullptr;
    QPushButton *load_project_button = nullptr;

    QLabel *project_icon_label;
    QPixmap project_icon;

public:
    project_button(button_settings &sett, QWidget *parent) : QWidget(parent)
    {
        setObjectName("project_button");

        settings = sett;

        QString style = QString("#project_button{background: %1; border: 1px solid black;}"
                                "#project_button:hover{background: %2; border: 1px solid black;}")
                            .arg(QString::fromStdString(sett.default_colour), QString::fromStdString(sett.hover_color));

        setStyleSheet(style);

        name_label = new QLabel(this);

        if (sett.pos == 0)
            name_label->setAlignment(Qt::AlignLeft);
        else if (sett.pos == 1)
            name_label->setAlignment(Qt::AlignRight);

        name_label->setFont(font);
        name_label->setText(QString::fromStdString(sett.name));

        make_project_button = new QPushButton(this);
        QPixmap make_pixmap("project_window/resourse/make_project_icon.png");
        QIcon make_ButtonIcon(make_pixmap);
        make_project_button->setIcon(make_ButtonIcon);
        make_project_button->setIconSize(QSize(30, 30));

        load_project_button = new QPushButton(this);
        QPixmap load_pixmap("project_window/resourse/open.jpg");
        QIcon load_ButtonIcon(load_pixmap);
        load_project_button->setIcon(load_ButtonIcon);
        load_project_button->setIconSize(QSize(40, 40));

        make_project_button->hide();
        load_project_button->hide();

        installEventFilter(this);

        connect(this, SIGNAL(MouseEnterSignal()), this, SLOT(OnMouseEnter()));
        connect(this, SIGNAL(MouseLeaveSignal()), this, SLOT(OnMouseLeave()));

        connect(make_project_button, SIGNAL(clicked()), this, SLOT(make_clicked()));
        connect(this, SIGNAL(create_project(int)), parent, SLOT(new_project(int)));

        project_icon_label = new QLabel(this);
        if (sett.type == project_type::network)
        {
            project_icon = QPixmap("project_window/resourse/network.png");
        }
        else
        {
            project_icon = QPixmap("project_window/resourse/sim.png");
        }
        project_icon_label->hide();
    }

    bool eventFilter(QObject *obj, QEvent *event)
    {
        Q_UNUSED(obj);
        switch (event->type())
        {
        case QEvent::Enter:
            emit MouseEnterSignal();
            break;
        case QEvent::Leave:
            emit MouseLeaveSignal();
            break;
        default:
            break;
        }
        return false;
    }

public:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        name_label->setGeometry(0, 20, width(), 70);
        make_project_button->setGeometry(2, height() - 50, width() / 2 - 2, 48);
        load_project_button->setGeometry(width() / 2 + 1, height() - 50, width() / 2 - 2, 48);
        project_icon_label->setGeometry((1 - settings.pos) * (width() * 0.75) + 20, 20, 70, 70);
        project_icon_label->setPixmap(project_icon.scaled(project_icon_label->width(), project_icon_label->height()));
    }

    void paintEvent(QPaintEvent *pe)
    {
        Q_UNUSED(pe);
        QStyleOption o;
        o.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PrimitiveElement::PE_Widget, &o, &p, this);
    }

public slots:
    void OnMouseEnter()
    {
        make_project_button->show();
        load_project_button->show();
        project_icon_label->show();
    }

    void OnMouseLeave()
    {
        make_project_button->hide();
        load_project_button->hide();
        project_icon_label->hide();
    }

    void make_clicked()
    {
        emit create_project((int)settings.type);
    }

signals:
    void MouseLeaveSignal();
    void MouseEnterSignal();
    void create_project(int type);
};