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

class object_dialog : public QDialog
{
    Q_OBJECT

public:
    object_dialog(QObject *parent)
    {
        connect(this, SIGNAL(need_update()), parent, SLOT(need_update()));
    }

public:
    Q_SLOT void update_params()
    {
        emit need_update();
    }

public:
    Q_SIGNAL void need_update();
};
