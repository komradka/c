#include "waitable_request.h"
#include "nd_pthread.h"

#include "con/tasks/task.h"

waitable_request::waitable_request ()
{
  m_mutex = std::make_unique<nd_mutex_t> ();
  m_cond_var = std::make_unique<nd_cond_var_t> ();
}

waitable_request::~waitable_request ()
{
}

bool waitable_request::is_free () const
{
  return !m_value;
}

bool waitable_request::request_value (task *value)
{
  m_mutex->lock ();

  if (!is_free ())
    return false;

  m_value = value;

  m_cond_var->wake_one ();

  m_mutex->unlock ();

  return true;
}

void waitable_request::request_exit ()
{
  m_mutex->lock ();

  m_exit = true;

  m_cond_var->wake_one ();

  m_mutex->unlock ();
}

task *waitable_request::wait ()
{
  m_mutex->lock ();

  while (!m_value && !m_exit)
    m_cond_var->wait (m_mutex.get ());

  if (m_exit)
    return nullptr;

  task *value = nullptr;
  std::swap (m_value, value);

  m_mutex->unlock ();

  return value;
}
