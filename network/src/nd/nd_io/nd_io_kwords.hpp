#include <string>

#pragma once

struct nd_project_kwords
{
    std::string res_num = "RES_NUM";
    std::string res_name = "RES_NAME";
    std::string gui_dir_path = "GUI_DIR";
    std::string settings_path = "SETTINGS";
    std::string pvt_dir_path = "PVT";
};

struct nd_gui_kwords
{
    std::string make_object = "MAKE_OBJECT";
    std::string make_link = "MAKE_LINK";
};

struct nd_kwords : public nd_project_kwords, public nd_gui_kwords
{

};
