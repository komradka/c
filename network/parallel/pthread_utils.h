#include <pthread.h>
#pragma once
class mutex_t
{
private:
  pthread_mutex_t m_mutex;

public:
  mutex_t () { m_mutex = PTHREAD_MUTEX_INITIALIZER; }
  ~mutex_t () = default;

  pthread_mutex_t *get () { return &m_mutex; }

  int lock () { return pthread_mutex_lock (&m_mutex); }
  int unlock () { return pthread_mutex_unlock (&m_mutex); }
};

class cond_var_t
{
private:
  pthread_cond_t m_cond_var;

public:
  cond_var_t () { m_cond_var = PTHREAD_COND_INITIALIZER; }
  ~cond_var_t () = default;

  int wait (mutex_t *mutex) { return pthread_cond_wait (&m_cond_var, mutex->get ()); }
  int wake_all () { return pthread_cond_broadcast (&m_cond_var); }
  int wake_one () { return pthread_cond_signal (&m_cond_var); }
};
