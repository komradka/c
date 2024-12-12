#include "events.h"

#include "../../managers/nd_manager.h"

events_queue::events_queue ()
{
  m_mutex = std::make_unique<mutex_t> ();
}

bool events_queue::queue_empty () const
{
  bool empty;

  m_mutex->lock ();
  empty = m_events.empty ();
  m_mutex->unlock ();

  return empty;
}

void events_queue::push (std::unique_ptr<event_t> event)
{
  m_mutex->lock ();
  m_events.push_back (std::move (event));
  m_mutex->unlock ();
  Q_EMIT received_event ();
}

std::unique_ptr<event_t> events_queue::pop ()
{
  if (queue_empty ())
    return {};

  m_mutex->lock ();

  std::unique_ptr<event_t> event = std::move (m_events.front ());
  m_events.pop_front ();

  m_mutex->unlock ();

  return std::move (event);
}

waker::waker (nd_manager &project)
  : m_project (project)
{
}

void waker::wake_up ()
{
  m_project.process_events ();
}
