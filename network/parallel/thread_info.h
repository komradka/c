#include <stdio.h>

#include "pthread_utils.h"

#pragma once

class thread_info
{
private:
  int m_thread_id;
  int m_threads_total;

public:
  thread_info (int thread_id, int threads_total);
  ~thread_info () = default;

  bool is_main_thread () const;

  int get_thread_id () const { return m_thread_id; }
  int get_threads_total () const { return m_threads_total; }

  void barrier () const;
  void reduce_sum (double *a, size_t len) const;
  void reduce_max (double *a) const;

  template<class Type>
  void bcast (Type **a) const
  {
    static mutex_t mutex;
    static cond_var_t cond_in, cond_out;
    static int in = 0, out = 0;
    static Type *res = nullptr;

    mutex.lock ();

    if (is_main_thread ())
      res = *a;

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

    if (!is_main_thread ())
      *a = res;

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
};

void get_uniform_range (int count, int &begin, int &end, const thread_info &thr_info);

