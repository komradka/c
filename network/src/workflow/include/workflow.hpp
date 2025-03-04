#include <map>
#include <string>

#pragma once

#include "error.hpp"
#include "wf_action.hpp"
#include "signal.hpp"
#include "report_system.hpp"

class workflow_dialog;
class wf_action_areaitem;

class workflow
{
private:
    std::map<int, wf_action *> actions;
    int action_count = 0;
    basic_action_storage basic_action;

public:
    workflow();
    ~workflow();

    void create_action(std::string name, std::string gui_name, std::string description, wf_action_category category,
                       types action_return_type, std::vector<types> &args, wf_action_utility utility);

    std::map<int, wf_action *> &get_actions() { return actions; }

    wf_action *get_action(int id) {return actions.at(id);}

    std::string get_info(workflow_dialog *);

    error calculate(report_system *, workflow_dialog *);

    error action_holder(wf_action_areaitem *);

    error check_args(wf_action_areaitem *);
};
