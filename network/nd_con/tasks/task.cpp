#include "task.h"

#include "parallel/thread_info.h"
#include "nd_con/async_reporter.h"

void task::set_reporter (async_reporter_t *reporter)
{
  m_reporter.reset (reporter);
}

void task::run (const thread_info &thr_info)
{
  if (thr_info.is_main_thread ())
    {
      m_reporter->print (MESSAGE, TOP_SECTION, "Started: Calculation \"%s\"", get_task_name ());
    }
  thr_info.barrier ();

  error err = multithread_run (thr_info);

  if (thr_info.is_main_thread ())
    {
      if (err.is_ok ())
        m_reporter->print (MESSAGE, TOP_SECTION, "Finished: Calculation \"%s\"", get_task_name ());
      else
        m_reporter->print (ERROR, TOP_SECTION, "Stopped: Calculation \"%s\"\nError: %s", get_task_name (), err.description_cstr ());
    }
}
