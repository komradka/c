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
                gui/sink_item.hpp \
                gui/scene.hpp \
                gui/link_item.hpp \
                gui/object_tree.hpp \
                gui/object_tree_item.hpp
                 

SOURCES       = main.cpp \
                reporter.cpp \
                reader/reader.cpp \
                kernel/topology.cpp \
                reader/topology_reader.cpp \
                kernel/objects_type.cpp \
                kernel/objects_data/source_data.cpp \
                kernel/objects_data/sink_data.cpp \
                gui/graph_area.cpp \
                gui/object_tree_item.cpp
                
                