#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>

class list_by_enum : public QListWidget
{
    Q_OBJECT

public:
    list_by_enum(QWidget *parent) : QListWidget(parent)
    {

    }

    template<typename e>
    void fill()
    {
        for (int i = 0; i < (int)e::COUNT; i++)
        {
            QListWidgetItem *item = new QListWidgetItem(this);
            item->setText(QString::fromStdString(get_name_for_gui((e)i)));
        }
    }
};