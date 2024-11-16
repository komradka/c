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

#include <optional>

#pragma once

class int_textEdit : public QLineEdit
{
    Q_OBJECT

public:
    int_textEdit(QWidget *parent = nullptr) : QLineEdit(parent)
    {
        setValidator(new QIntValidator);
    }

    std::optional<int> get_num()
    {
        QString t = text();
        bool is_ok = false;

        int ret = t.toInt(&is_ok);

        return is_ok ? std::make_optional(ret) : std::nullopt;
    }
};

class double_textEdit : public QLineEdit
{
    Q_OBJECT

public:
    double_textEdit(QWidget *parent = nullptr) : QLineEdit(parent)
    {
        setValidator(new QDoubleValidator);
    }

    std::optional<double> get_num()
    {
        QString t = text();
        bool is_ok = false;

        double ret = t.toDouble(&is_ok);

        return is_ok ? std::make_optional(ret) : std::nullopt;
    }
};