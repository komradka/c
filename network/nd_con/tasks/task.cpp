#include "task.h"

#include "parallel/thread_info.h"
#include "nd_con/async_reporter.h"
#include "managers/nd_manager.h"

task::task (nd_manager &manager) : m_manager (manager)
{
}

void task::from_manager (nd_manager &manager)
{
  m_comanager.reset (create_comanager (manager));
}

void task::set_reporter (async_reporter_t *reporter)
{
  m_reporter.reset (reporter);
}

error task::verify_before_run () const
{
  return error (OK);
}

void task::run (const thread_info &thr_info)
{
  if (thr_info.is_main_thread ())
    {
      m_reporter->print (MESSAGE, TOP_SECTION, "Started: Calculation \"%s\".", get_task_name ());
    }
  thr_info.barrier ();

  error err = multithread_run (thr_info);

  if (thr_info.is_main_thread ())
    {
      if (err.is_ok ())
        {
          m_reporter->print (MESSAGE, TOP_SECTION, "Finished: Calculation \"%s\".", get_task_name ());

          std::unique_ptr<task_done_event> event (new task_done_event (m_manager, this));
          m_manager.get_events_queue ()->push (std::move (event));
        }
      else
        m_reporter->print (ERROR, TOP_SECTION, "Stopped: Calculation \"%s\".\nError: %s.", get_task_name (), err.description_cstr ());
    }
}

void task::after_finish (nd_manager &manager) const
{
  for (auto func : m_do_after_finish_in_gui)
    func (manager);
}

void task::do_after_finish_in_gui (const std::function<void (nd_manager &manager)> &func)
{
   m_do_after_finish_in_gui.push_back (func);
}

task_done_event::task_done_event (nd_manager &manager, task *f)
  : m_manager (manager), m_task (f)
{
}

void task_done_event::process ()
{
  m_task->after_finish (m_manager);
}
