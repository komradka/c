#include <iostream>
#include <vector>
#include <map>

#include "../kernel/fluid/water_props_desc.hpp"
#include "../gui_utility/tree_dialog_by_enum.hpp"
#include "fluid_experiment_dialog.hpp"
#include "fluid_graph/fluid_graph.hpp"

#pragma once

class fluid_widget : public QWidget
{
    Q_OBJECT

private:
    static constexpr unsigned int props_count = (unsigned int)(water_props::COUNT);

    static constexpr std::array<water_props, int(water_props::COUNT)> all_props = {water_props::reference_pressure,
                                                                                   water_props::FVF,
                                                                                   water_props::compressibility,
                                                                                   water_props::viscosity,
                                                                                   water_props::viscosibility,
                                                                                   water_props::density,
                                                                                   water_props::density_correlation,
                                                                                   water_props::viscosity_correlation};

    std::map<water_props, std::any> props;
    tree_dialog_by_enum *td;
    experiment_dialog *experiment;
    fluid_plot *plot;

public:
    fluid_widget()
    {
        if (all_props.size() != props_count)
        {
            abort();
        }

        td = new tree_dialog_by_enum(this->width(), this);
        experiment = new experiment_dialog(this);
        plot = new fluid_plot(this);

        auto create = [&](auto i)
        {
            td->add_widget<water_props_desc<all_props[i]>>();
        };

        constexpr_for<0, props_count>(create);

        update_params();

        setContextMenuPolicy(Qt::CustomContextMenu);

        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(show_menu(const QPoint &)));
    }

    template <water_props Props>
    auto get_param() -> typename water_props_desc<Props>::data_type_t
    {
        using data_type_t = typename water_props_desc<Props>::data_type_t;
        auto value = props[Props];

        if (!value.has_value())
        {
            abort();
        }

        return std::any_cast<data_type_t>(value);
    }

public slots:
    void update_params()
    {
        auto set_settings = [&](auto i)
        {
            props[all_props[i]] = td->get_widget_param<water_props_desc<all_props[i]>>(i);
        };

        constexpr_for<0, props_count>(set_settings);
    }

    void show_menu(const QPoint &pos)
    {
        QMenu cMenu("Actions", this);

        QAction experiment_act("Make Experiment", this);
        connect(&experiment_act, SIGNAL(triggered()), this, SLOT(make_experiment()));
        cMenu.addAction(&experiment_act);

        QAction graph_act("Props Graph", this);
        connect(&graph_act, SIGNAL(triggered()), this, SLOT(make_graph()));
        cMenu.addAction(&graph_act);

        cMenu.exec(mapToGlobal(pos));
    }

    void make_experiment()
    {
        experiment->show();
    }

    void make_graph()
    {
        plot->show();
    }

private:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        td->setGeometry(0, 0, width(), height());
    }
};