#include "CALCULATE_task.h"

#include "nd/iface/nd_manager.h"

#include "thread_info.h"
#include "nd/queue/waitable_request.h"
#include "nd/queue/thread_class.h"
#include "thread_shared_ptr.h"
#include "nd/iface/async_reporter.h"

#include "nd/kernel/network_solver/nd_solver.hpp"

const char *CALCULATE_task::get_task_name () const
{
  return "Calculate";
}

nd_temporary_storage *CALCULATE_task::create_comanager (nd_manager &manager) const
{
  nd_temporary_storage *comanager = new nd_temporary_storage;

  graph *topology = manager.get_network_topology ();
  if (topology)
    {
      graph *cotopology = new graph;
      cotopology->copy_from (topology);
      comanager->topology = cotopology;
    }

  settings_dialog *settings = manager.get_settings ();
  if (settings)
    {
      settings_dialog *cosettings = new settings_dialog ();
      cosettings->copy_from (settings);
      comanager->sett = cosettings;
    }

  fluid_props *fluid = manager.get_fluid();
  if (fluid)
  {
    fluid_props *cofluid =  new fluid_props();
    cofluid->copy_from(fluid);
    comanager->fluid = fluid;
  }

  return comanager;
}

error CALCULATE_task::verify_before_run () const
{
  if (!m_comanager->fluid)
      return error ("Choose fluid for network");

  graph *topology = m_comanager->topology;
  if (!topology)
    return error ("Make network topology first.");

  error err = topology->verify_network_before_calculation ();
  if (!err.is_ok ())
    return err;

  return error (OK);
}

error CALCULATE_task::multithread_run (const thread_info &thr_info)
{
  thread_shared_ptr<nd_solver> solver (thr_info, m_reporter.get (), m_comanager->topology, m_comanager->sett, m_comanager->fluid);
  error err = solver->run (thr_info);

  if (!err.is_ok ())
    return err;

  if (thr_info.is_main_thread ())
    {
      do_after_finish_in_gui ([results = solver->get_object_results ()] (nd_manager &manager)
      {
          manager.copy_results (results);
      });
    }

  thr_info.barrier ();
  return error (OK);
}

