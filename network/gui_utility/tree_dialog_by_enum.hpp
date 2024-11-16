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

#include "../utility/utility.hpp"
#include "number_textEditer.hpp"
#include "number_textEditer.hpp"
#include <map>
#include <any>
#include <array>
#include <type_traits>

#include <iostream>

#pragma once

class tree_dialog_by_enum : public QTreeWidget
{
    Q_OBJECT

public:
    static const int widget_column = 1;
    static const int text_column = 0;

private:
    std::vector<QWidget *> items;

public:
    tree_dialog_by_enum(int width, QWidget *parent) : QTreeWidget(parent)
    {
        setColumnCount(2);
        header()->resizeSection(0, width / 2);
        QString style = "QTreeWidget::item:!selected "
                        "{ "
                        "border: 2px solid gainsboro; "
                        "border-left: none; "
                        "border-top: none; "
                        "}"
                        "QTreeWidget::item:selected {}";

        setStyleSheet(style);

        setHeaderLabels({"Parameter", "Value"});

        connect(this, SIGNAL(update_param()), parent, SLOT(update_params()));
    }

    template <typename Desc>
    void add_widget()
    {
        using data_type_t = typename Desc::data_type_t;

        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        addTopLevelItem(item);
        item->setText(text_column, QString::fromStdString(Desc::name_for_gui));

        if constexpr (std::is_same<data_type_t, bool>::value == true)
        {
            add_bool_widget(&Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, std::optional<bool>>::value == true)
        {
            bool_optional_holder(Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, int>::value == true)
        {
            add_int_widget(&Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, std::optional<int>>::value == true)
        {
            int_optional_holder(Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, double>::value == true)
        {
            add_double_widget(&Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, std::optional<double>>::value == true)
        {
            double_optional_holder(Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, std::string>::value == true)
        {
            add_string_widget(&Desc::default_type, item);
            return;
        }
        else if constexpr (std::is_same<data_type_t, std::optional<std::string>>::value == true)
        {
            string_optional_holder(Desc::default_type, item);
            return;
        }
        else
        {
            add_enum_widget<Desc>(item);
            return;
        }
    }

private:
    void bool_optional_holder(const std::optional<bool> dv, QTreeWidgetItem *item)
    {
        add_bool_widget(dv ? &dv.value() : nullptr, item);
    }

    void add_bool_widget(const bool *dv, QTreeWidgetItem *item)
    {
        QCheckBox *box = new QCheckBox();
        dv ? box->setChecked(*dv) : box->setChecked(false);
        items.push_back(box);
        setItemWidget(item, widget_column, box);
        connect(box, SIGNAL(stateChanged(int)), this, SLOT(change_int(int)));
    }

    void int_optional_holder(const std::optional<int> dv, QTreeWidgetItem *item)
    {
        add_int_widget(dv ? &dv.value() : nullptr, item);
    }

    void add_int_widget(const int *dv, QTreeWidgetItem *item)
    {
        int_textEdit *line = new int_textEdit();
        if (dv)
            line->setText(QString::number(*dv));
        items.push_back(line);
        setItemWidget(item, widget_column, line);
        connect(line, SIGNAL(textChanged(const QString &)), this, SLOT(change_str(const QString &)));
    }

    void double_optional_holder(const std::optional<double> dv, QTreeWidgetItem *item)
    {
        add_double_widget(dv ? &dv.value() : nullptr, item);
    }

    void add_double_widget(const double *dv, QTreeWidgetItem *item)
    {
        double_textEdit *line = new double_textEdit();
        if (dv)
            line->setText(QString::number(*dv));
        items.push_back(line);
        setItemWidget(item, widget_column, line);
        connect(line, SIGNAL(textChanged(const QString &)), this, SLOT(change_str(const QString &)));
    }

    void string_optional_holder(const std::optional<std::string> dv, QTreeWidgetItem *item)
    {
        add_string_widget(dv ? &dv.value() : nullptr, item);
    }

    void add_string_widget(const std::string *dv, QTreeWidgetItem *item)
    {
        QLineEdit *line = new QLineEdit();
        if (dv)
            line->setText(QString::fromStdString(*dv));
        items.push_back(line);
        setItemWidget(item, widget_column, line);
        connect(line, SIGNAL(textChanged(const QString &)), this, SLOT(change_str(const QString &)));
    }

    template <typename Desc>
    void add_enum_widget(QTreeWidgetItem *item)
    {
        using data_type_t = typename Desc::data_type_t;
        QComboBox *box = new QComboBox();

        for (int i = 0; i < (int)data_type_t::COUNT; i++)
        {
            box->addItem(QString::fromStdString(get_name_for_gui((data_type_t)(i))));
        }

        box->setCurrentIndex((int)Desc::default_type);

        items.push_back(box);
        setItemWidget(item, widget_column, box);
        connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(change_int(int)));
    }

public slots:
    void change_int(int i)
    {
        Q_UNUSED(i);
        emit update_param();
    }

    void change_str(const QString &str)
    {
        Q_UNUSED(str);
        emit update_param();
    }

signals:
    void update_param();

public:
    template <typename Desc>
    auto get_widget_param(int i) -> typename Desc::data_type_t
    {
        using data_type_t = typename Desc::data_type_t;

        if constexpr (std::is_same<data_type_t, bool>::value == true)
        {
            QCheckBox *check_box = dynamic_cast<QCheckBox *>(items[i]);
            Qt::CheckState state = check_box->checkState();
            return state == Qt::Checked ? true : false;
        }
        else if constexpr (std::is_same<data_type_t, std::optional<bool>>::value == true)
        {
            QCheckBox *check_box = dynamic_cast<QCheckBox *>(items[i]);
            Qt::CheckState state = check_box->checkState();
            return state == Qt::Checked ? std::make_optional(true) : std::make_optional(false);
        }
        else if constexpr (std::is_same<data_type_t, int>::value == true)
        {
            int_textEdit *text = dynamic_cast<int_textEdit *>(items[i]);
            return text->get_num().value();
        }
        else if constexpr (std::is_same<data_type_t, std::optional<int>>::value == true)
        {
            int_textEdit *text = dynamic_cast<int_textEdit *>(items[i]);
            return text->get_num();
        }
        else if constexpr (std::is_same<data_type_t, double>::value == true)
        {
            double_textEdit *text = dynamic_cast<double_textEdit *>(items[i]);
            return text->get_num().value();
        }
        else if constexpr (std::is_same<data_type_t, std::optional<double>>::value == true)
        {
            double_textEdit *text = dynamic_cast<double_textEdit *>(items[i]);
            return text->get_num();
        }
        else if constexpr (std::is_same<data_type_t, std::string>::value == true)
        {
            QLineEdit *text = dynamic_cast<QLineEdit *>(items[i]);
            return text->text().toStdString();
        }
        else if constexpr (std::is_same<data_type_t, std::optional<std::string>>::value == true)
        {
            QLineEdit *text = dynamic_cast<QLineEdit *>(items[i]);
            return std::make_optional(text->text().toStdString());
        }
        else
        {
            QComboBox *combo_box = dynamic_cast<QComboBox *>(items[i]);
            return (data_type_t)(combo_box->currentIndex());
        }
    }

    template <typename Desc>
    void set_widget_param(int i, typename Desc::data_type_t value)
    {
        using data_type_t = typename Desc::data_type_t;

        if constexpr (std::is_same<data_type_t, bool>::value == true)
        {
            QCheckBox *check_box = dynamic_cast<QCheckBox *>(items[i]);
            check_box->setChecked(value);
        }
        else if constexpr (std::is_same<data_type_t, std::optional<bool>>::value == true)
        {
            QCheckBox *check_box = dynamic_cast<QCheckBox *>(items[i]);
            if (value)
                check_box->setChecked(value.value());
            else
                check_box->setChecked(false);
        }
        else if constexpr (std::is_same<data_type_t, int>::value == true)
        {
            int_textEdit *text = dynamic_cast<int_textEdit *>(items[i]);
            text->setText(QString::number(value));
        }
        else if constexpr (std::is_same<data_type_t, std::optional<int>>::value == true)
        {
            int_textEdit *text = dynamic_cast<int_textEdit *>(items[i]);
            if (value)
                text->setText(QString::number(value.value()));
            else
                text->clear();
        }
        else if constexpr (std::is_same<data_type_t, double>::value == true)
        {
            double_textEdit *text = dynamic_cast<double_textEdit *>(items[i]);
            text->setText(QString::number(value));
        }
        else if constexpr (std::is_same<data_type_t, std::optional<double>>::value == true)
        {
            double_textEdit *text = dynamic_cast<double_textEdit *>(items[i]);
            if (value)
                text->setText(QString::number(value.value()));
            else
                text->clear();
        }
        else if constexpr (std::is_same<data_type_t, std::string>::value == true)
        {
            QLineEdit *text = dynamic_cast<QLineEdit *>(items[i]);
            text->setText(QString::fromStdString(value));
        }
        else if constexpr (std::is_same<data_type_t, std::optional<std::string>>::value == true)
        {
            QLineEdit *text = dynamic_cast<QLineEdit *>(items[i]);
            if (value)
                text->setText(QString::fromStdString(value.value()));
            else
                text->clear();
        }
        else
        {
            QComboBox *combo_box = dynamic_cast<QComboBox *>(items[i]);
            combo_box->setCurrentIndex((int)value);
        }
    }
};