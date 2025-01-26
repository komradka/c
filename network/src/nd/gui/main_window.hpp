#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtWidgets/QtWidgets>
#include <QtGui/QtGui>
#include <iostream>

using namespace std;

class MWindow : public QMainWindow
{
    Q_OBJECT
public:
    QObject *pm;

public:
    MWindow(QWidget *parent)
    {
        pm = parent;
    }
    QSize minimumSizeHint() const
    {
        return QSize(600, 100);
    }
    QSize maximumSize() const
    {
        return QSize(2000, 2000);
    }
    QSize sizeHint() const
    {
        return QSize(1000, 1000);
    }
};