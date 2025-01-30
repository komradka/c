#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

#pragma once

class enum_widget : public QComboBox
{
    Q_OBJECT

public:
    enum_widget(QWidget *parent = nullptr) : QComboBox(parent)
    {
    }

    template <typename Enum>
    void fill_widget(Enum default_value = (Enum)0)
    {
        for (int i = 0; i < (int)Enum::COUNT; i++)
        {
            addItem(QString::fromStdString(get_name_for_gui(Enum(i))));
        }

        setCurrentIndex((int)default_value);
    }
};
