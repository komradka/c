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
#include <vector>

class fluid_list_item : public QListWidgetItem
{
private:
public:
    bool is_selected = false;
    int id;

    fluid_list_item(std::string name, int id)
    {
        setText(QString::fromStdString(name));
        this->id = id;
    }
};