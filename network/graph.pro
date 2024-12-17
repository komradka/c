QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS -= -W -Wall

HEADERS       = manager.hpp \
                main_window.hpp \
                parallel/pthread_utils.h \
                parallel/thread_class.h \
                parallel/thread_shared_ptr.h \
                parallel/waitable_request.h \
                reader/reader.hpp \
                reporter.hpp \
                kernel/topology.hpp \
                error.hpp \
                kernel/objects_data/object_data.hpp \
                kernel/objects_data/sink_data.hpp \
                kernel/objects_data/source_data.hpp \
                kernel/objects_type.hpp  \
                reader/objects_data_reader.hpp \
                gui/graph_area.hpp \
                gui/graph_item.hpp \
                gui/source_item.hpp \
                gui/pipe_item.hpp \
                gui/sink_item.hpp \
                gui/scene.hpp \
                gui/link_item.hpp \
                gui/object_tree.hpp \
                gui/object_tree_item.hpp \
                writer/writer.hpp \
                kernel/objects_data/object_params.hpp \
                utility/utility.hpp \
                manager_gui/save_dialog.hpp \
                manager_gui/load_dialog.hpp \
                manager_gui/result_info_storage.hpp \
                manager_gui/result_tree_item.hpp \
                manager_gui/settings_dialog.hpp \
                utility/network_statistic.hpp \
                kernel/check_network.hpp  \
                manager_gui/settings/solver_settings_desc.hpp \
                manager_gui/settings/solver_settings_enum.hpp \
                gui_utility/number_textEditer.hpp \
                gui_utility/tree_dialog_by_enum.hpp \
                gui/object_dialog.hpp \
                kernel/objects_data/pipe_data.hpp \
                kernel/objects_data/object_param_enum.hpp \
                gui/result_widget.hpp \
                kernel/phys_q_type.hpp \
                kernel/network_solver/nd_solver.hpp \
                kernel/network_solver/nd_solution.hpp \
                kernel/network_solver/initial_approximation/initial_approximation_builder.hpp \
                kernel/network_solver/nd_results.hpp \
                kernel/phys_q.hpp \
                kernel/network_solver/matrix_builder/jacobian_row.hpp \
                kernel/network_solver/matrix_builder/row_element.hpp \
                kernel/network_solver/matrix_builder/matrix_builder.hpp \
                kernel/network_solver/equations/nd_abstract_equation.hpp \
                kernel/network_solver/equations/nd_source_pressure_equation.hpp \
                kernel/network_solver/equations/nd_source_rate_equation.hpp \
                kernel/network_solver/equations/nd_pipe_pressure_equation.hpp \
                kernel/network_solver/main_equations.hpp \
                kernel/network_solver/equations/nd_equality_pressure_equation.hpp \
                kernel/network_solver/equations/nd_equality_rate_equation.hpp \
                kernel/fluid/water_props_desc.hpp \
                kernel/fluid/water_props.hpp \
                gui/fluid_widget.hpp \
                gui_utility/enum_widget.hpp \
                gui/fluid_experiment_dialog.hpp \
                kernel/fluid/experiment/fluid_experiment.hpp \
                kernel/fluid/calculators/water_calculator.hpp \
                gui/fluid_graph/fluid_graph.hpp \
                gui_utility/plot/qcustomplot.h \
                gui/pipe_project/pipe_project.hpp \
                gui/pipe_project/pipe_graph.hpp \
                gui_utility/check_list_widget.hpp \
                kernel/pipe_holder/pipe_params.hpp \
                kernel/pipe_holder/pipe_calculator.hpp \
                kernel/objects_data/joint_data.hpp \
                gui/joint_item.hpp \
                manager_gui/create_project_dialog.hpp \
                parallel/thread_info.h \
                gui/gui_feedback/events.h \
                nd_con/async_reporter.h \
                nd_con/tasks/task.h \
                argsparser.h \
                project_window/project_main_window.hpp \
                project_window/project_button.hpp \
                managers/project_manager.hpp \
                managers/nd_manager.h \
                project_window/nd_window.hpp \
                utility/signal_slots/signal.hpp \
                utility/signal_slots/slot.hpp


SOURCES       = main.cpp \
                argsparser.cpp \
                nd_con/async_reporter.cpp \
                nd_con/tasks/task.cpp \
                gui/gui_feedback/events.cpp \
                parallel/thread_class.cpp \
                parallel/thread_info.cpp \
                parallel/thread_shared_ptr.cpp \
                parallel/waitable_request.cpp \
                reporter.cpp \
                reader/reader.cpp \
                kernel/topology.cpp \
                reader/topology_reader.cpp \
                kernel/objects_type.cpp \
                kernel/objects_data/source_data.cpp \
                kernel/objects_data/sink_data.cpp \
                gui/graph_area.cpp \
                gui/object_tree_item.cpp \
                utility/signal_slots/signal.cpp \
                writer/writer.cpp \
                kernel/check_network.cpp \
                manager_gui/settings/solver_settings_enum.cpp \
                utility/utility.cpp \
                kernel/objects_data/pipe_data.cpp \
                kernel/objects_data/object_param_enum.cpp \
                kernel/phys_q_type.cpp \
                gui/result_widget.cpp \
                kernel/network_solver/nd_solver.cpp \
                kernel/network_solver/nd_solution.cpp \
                kernel/network_solver/initial_approximation/pressure_initial_approximation.cpp \
                kernel/network_solver/initial_approximation/temp_initial_approximation.cpp \
                kernel/network_solver/initial_approximation/rate_initial_approximation.cpp \
                kernel/network_solver/equations/nd_pipe_pressure_equation.cpp \
                kernel/network_solver/main_equations.cpp \
                kernel/fluid/water_props.cpp \
                kernel/internal_constant.hpp \
                kernel/fluid/experiment/fluid_experiment.cpp \
                kernel/fluid/calculators/water_calculator.cpp \
                gui/fluid_experiment_dialog.cpp \
                gui_utility/plot/qcustomplot.cpp \
                gui/fluid_graph/fluid_graph.cpp \
                kernel/pipe_holder/pipe_params.cpp \
                kernel/pipe_holder/pipe_calculator.cpp \
                kernel/objects_data/joint_data.cpp \
                kernel/objects_data/object_data.cpp \
                reader/settings_reader.cpp \
                managers/project_manager.cpp \
                managers/nd_manager.cpp \
                project_window/nd_window.cpp
                
