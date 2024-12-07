#include <pthread.h>

#include "thread_info.h"

thread_info::thread_info (int thread_id, int threads_total)
  : m_thread_id (thread_id), m_threads_total (threads_total)
{
}

bool thread_info::is_main_thread () const
{
  return m_thread_id == 0;
}

void thread_info::barrier () const
{
  static nd_mutex_t mutex;
  static nd_cond_var_t cond_in, cond_out;
  static int in = 0, out = 0;

  mutex.lock ();

  in++;
  if (in >= m_threads_total)
    {
      out = 0;
      cond_in.wake_all ();
    }
  else
    {
      while (in < m_threads_total)
        cond_in.wait (&mutex);
    }

  out++;
  if (out >= m_threads_total)
    {
      in = 0;
      cond_out.wake_all ();
    }
  else
    {
      while (out < m_threads_total)
        cond_out.wait (&mutex);
    }

  mutex.unlock ();
}

void thread_info::reduce_sum (double *a, size_t len) const
{
  static nd_mutex_t mutex;
  static nd_cond_var_t cond_in, cond_out;
  static int in = 0, out = 0;
  static double *res = nullptr;

  mutex.lock ();

  if (!res)
    {
      res = a;
    }
  else
    {
      for (size_t i = 0; i < len; i++)
        res[i] += a[i];
    }

  in++;
  if (in >= m_threads_total)
    {
      out = 0;
      cond_in.wake_all ();
    }
  else
    {
      while (in < m_threads_total)
        cond_in.wait (&mutex);
    }

  if (res != a)
    {
      for (size_t i = 0; i < len; i++)
        a[i] = res[i];
    }

  out++;
  if (out >= m_threads_total)
    {
      in = 0;
      res = nullptr;
      cond_out.wake_all ();
    }
  else
    {
      while (out < m_threads_total)
        cond_out.wait (&mutex);
    }

  mutex.unlock ();
}

void thread_info::reduce_max (double *a) const
{
  static nd_mutex_t mutex;
  static nd_cond_var_t cond_in, cond_out;
  static int in = 0, out = 0;
  static double res = 0;

  mutex.lock ();

  if (in == 0)
    res = *a;
  else
    {
      if (res < *a)
        res = *a;
    }

  in++;
  if (in >= m_threads_total)
    {
      out = 0;
      cond_in.wake_all ();
    }
  else
    {
      while (in < m_threads_total)
        cond_in.wait (&mutex);
    }

  *a = res;

  out++;
  if (out >= m_threads_total)
    {
      in = 0;
      res = 0;
      cond_out.wake_all ();
    }
  else
    {
      while (out < m_threads_total)
        cond_out.wait (&mutex);
    }

  mutex.unlock ();
}

void get_uniform_range (int count, int &begin, int &end, const thread_info &thr_info)
{
  int thr_id = thr_info.get_thread_id ();
  int thr_total = thr_info.get_threads_total ();

  begin = thr_id * count / thr_total;
  end = (thr_id + 1) * count / thr_total;
}
