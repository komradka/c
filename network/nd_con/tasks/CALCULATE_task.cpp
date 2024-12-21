#include "CALCULATE_task.h"

#include "managers/nd_manager.h"

#include "parallel/thread_info.h"
#include "parallel/waitable_request.h"
#include "parallel/thread_class.h"
#include "nd_con/async_reporter.h"

#include "kernel/network_solver/nd_solver.hpp"

const char *CALCULATE_task::get_task_name () const
{
  return "Calculate";
}

nd_manager *CALCULATE_task::create_comanager (nd_manager &manager) const
{
  nd_manager *comanager = new nd_manager;

  graph *topology = manager.get_network_topology ();
  if (topology)
    {
      graph *cotopology = new graph;
      cotopology->copy_from (topology);
      comanager->set_network_topology (cotopology);
    }

  settings_dialog *settings = manager.get_settings ();
  if (settings)
    {
      settings_dialog *cosettings = new settings_dialog ();
      cosettings->copy_from (settings);
      comanager->set_settings (cosettings);
    }

  return comanager;
}

error CALCULATE_task::verify_before_run () const
{
  graph *topology = m_manager->get_network_topology ();
  if (!topology)
    return error ("Make network topology first.");

  error err = topology->verify_network_before_calculation ();
  if (!err.is_ok ())
    return err;

  return error (OK);
}

error CALCULATE_task::multithread_run (const thread_info &thr_info)
{


  return error (OK);
}

