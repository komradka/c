QT += widgets
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS -= -W -Wall

HEADERS       = manager.hpp \
                main_window.hpp \
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
                kernel/network_solver/equations/nd_abstract_equation.hpp \
                kernel/network_solver/equations/nd_source_pressure_equation.hpp
                

                 

SOURCES       = main.cpp \
                reporter.cpp \
                reader/reader.cpp \
                kernel/topology.cpp \
                reader/topology_reader.cpp \
                kernel/objects_type.cpp \
                kernel/objects_data/source_data.cpp \
                kernel/objects_data/sink_data.cpp \
                gui/graph_area.cpp \
                gui/object_tree_item.cpp \
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
                kernel/network_solver/initial_approximation/rate_initial_approximation.cpp
                
                