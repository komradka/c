#include "fluid_dialog.hpp"
#include "nd/gui/nd_window.hpp"

void fluid_dialog::add_fluid()
{
    bool ok;

    QString text = QInputDialog::getText(this, QString::fromUtf8("PVT creating"),
                                         QString::fromUtf8("PVT project name:"),
                                         QLineEdit::Normal,
                                         "Fluid " + QString::number(list->count() + 1),
                                         &ok);

    if (ok && !text.isEmpty())
    {
        std::string pvt_name = text.toStdString();
        std::replace(pvt_name.begin(), pvt_name.end(), ' ', '_');

        nd_window->make_fluid(pvt_name);

        list->add_fluid(list->count(), pvt_name);
    }
}

void fluid_dialog::change_fluid()
{
    nd_window->set_fluid(list->selected_fluid->id);
}

void fluid_dialog::export_fluid(std::string name, std::string path)
{
    std::replace(name.begin(), name.end(), ' ', '_');

    nd_window->export_fluid(name, path);

    list->add_fluid(list->count(), name);
}

#include "moc_fluid_dialog.cpp"
