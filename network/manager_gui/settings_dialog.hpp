#include "settings/solver_settings_desc.hpp"
#include "../gui_utility/tree_dialog_by_enum.hpp"
#include <fstream>

#include <iostream>

#pragma once

template <solver_settings Settings>
struct network_solver_setting_desc;

class settings_dialog : public QDialog
{
    Q_OBJECT

public:
    static constexpr unsigned int setting_count = (unsigned int)(solver_settings::COUNT);

    static constexpr std::array<solver_settings, int(solver_settings::COUNT)> all_settings = {solver_settings::use_temperature_equation,
                                                                                              solver_settings::use_heat_balance_equation,
                                                                                              solver_settings::visualize_initial_approximation,

                                                                                              solver_settings::solver_type,
                                                                                              solver_settings::linear_solver_type,
                                                                                              solver_settings::initial_approximation_type,
                                                                                              solver_settings::preconditioner_type,
                                                                                              solver_settings::rate_type,

                                                                                              solver_settings::max_linear_iter,
                                                                                              solver_settings::max_newton_iter,

                                                                                              solver_settings::linear_solver_eps,
                                                                                              solver_settings::rhs_eps,
                                                                                              solver_settings::root_difference_eps};

    std::map<solver_settings, std::any> settings;
    tree_dialog_by_enum *td;

public:
    settings_dialog(QObject *parent)
    {
        Q_UNUSED(parent);
        if (all_settings.size() != setting_count)
        {
            abort();
        }

        init_default();

        td = new tree_dialog_by_enum(700, this);

        setFixedSize(700, 700);
        td->setGeometry(0, 0, 700, 700);

        auto create = [&](auto i)
        {
            td->add_widget<network_solver_setting_desc<all_settings[i]>>();
        };

        constexpr_for<0, setting_count>(create);
    }

    template <solver_settings Settings>
    auto get_param() -> typename network_solver_setting_desc<Settings>::data_type_t
    {
        using data_type_t = typename network_solver_setting_desc<Settings>::data_type_t;
        auto value = settings[Settings];

        if (!value.has_value())
        {
            abort();
        }

        return std::any_cast<data_type_t>(value);
    }

    void write_settings(std::ofstream &stream)
    {
        auto write = [&](auto i)
        {
            stream << network_solver_setting_desc<all_settings[i]>::name << " ";
            stream << get_name(get_param<all_settings[i]>()) << std::endl;
        };

        constexpr_for<0, setting_count>(write);
    }

    template <solver_settings Settings>
    void set_param(typename network_solver_setting_desc<Settings>::data_type_t value, int i)
    {
        td->set_widget_param<network_solver_setting_desc<Settings>>(i, value);
    }

public slots:
    void update_params()
    {
        auto set_settings = [&](auto i)
        {
            settings[all_settings[i]] = td->get_widget_param<network_solver_setting_desc<all_settings[i]>>(i);
        };

        constexpr_for<0, setting_count>(set_settings);
    }

private:
    void init_default()
    {
        auto init = [&](auto i)
        {
            settings[all_settings[i]] = network_solver_setting_desc<all_settings[i]>::default_type;
        };

        constexpr_for<0, setting_count>(init);
    }
};