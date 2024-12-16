#include "nd_window.hpp"
#include "../managers/nd_manager.h"

void nd_main_window::closeEvent(QCloseEvent *event)
{
    pm->project_closed();
    event->accept();
}