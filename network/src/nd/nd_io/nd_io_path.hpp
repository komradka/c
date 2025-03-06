#include <string>

#pragma once

struct nd_io_path
{
    std::string gui_dir_path = "GUI";
    std::string gui_data_path = "GUI/GUI.data";
    std::string settings_data_path = "settings.data";
    std::string pvt_dir_path = "PVT";
    std::string pvt_data_path = "PVT/PVT.data";
};

std::string get_datafile_path(std::string object_name);
