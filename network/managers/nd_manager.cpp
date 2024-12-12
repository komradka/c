#include "nd_manager.h"

#include "../nd_con/tasks/task.h"
#include "../parallel/thread_class.h"
#include "../parallel/waitable_request.h"

#include "../gui/gui_feedback/events.h"

#include "../nd_con/async_reporter.h"

#include "../manager.hpp"

nd_manager::nd_manager ()
{
  m_events_queue = std::make_unique<events_queue> ();
  m_waker = std::make_unique<waker> (*this);
  QObject::connect (m_events_queue.get (), &events_queue::received_event, m_waker.get (), &waker::wake_up, Qt::QueuedConnection);
}

nd_manager::~nd_manager ()
{
  m_task_queue->request_exit ();
  m_task_threads->join ();
}

int nd_manager::get_total_threads () const
{
  return m_total_threads;
}

void nd_manager::set_manager (manager *mgr)
{
  m_manager = mgr;
}

events_queue *nd_manager::get_events_queue ()
{
  return m_events_queue.get ();
}

void nd_manager::process_events ()
{
  while (auto event = m_events_queue->pop ())
    event->process ();
}

bool nd_manager::add_task (task *f)
{
  f->set_reporter (new async_reporter_t (*this));
  return m_task_queue->request_value (f);
}

void nd_manager::create_kernel_threads (int total_threads)
{
  m_total_threads = total_threads;

  m_task_queue = std::make_unique<waitable_request> ();
  m_task_threads = std::make_unique<thread_group_class> ("ND:", total_threads, m_task_queue.get ());

  m_task_threads->start ();
}

void nd_manager::process_print_log (message_t to_print)
{
  m_manager->print_log (to_print);
}
