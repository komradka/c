#include <pthread.h>

#include "thread_info.h"

thread_info::thread_info (int thread_id, int threads_total)
  : m_thread_id (thread_id), m_threads_total (threads_total)
{
}

void thread_info::barrier () const
{
  static mutex_t mutex;
  static cond_var_t cond_in, cond_out;
  static int in = 0, out = 0;

  mutex.lock ();

  in++;
  if (in >= m_threads_total)
    {
      out = 0;
      cond_in.wake_all ();
    }
  else
    while (in < m_threads_total)
      cond_in.wait (&mutex);

  out++;
  if (out >= m_threads_total)
    {
      in = 0;
      cond_out.wake_all ();
    }
  else
    while (out < m_threads_total)
      cond_out.wait (&mutex);

  mutex.unlock ();
}

error thread_info::reduce_error (error &err) const
{
  int root = get_threads_total ();
  if (!err.is_ok ())
    root = get_thread_id ();

  allreduce_min (root);

  if (root < get_threads_total ())
    {
      std::string error_message = err.description ();
      size_t error_message_length = error_message.length ();

      bcast (error_message_length, root);

      if (get_thread_id () != root)
        error_message.resize (error_message_length);

      bcast (error_message.data (), error_message_length, root);

      if (get_thread_id () != root)
        err = error (error_message);

      barrier ();
    }
  return err;
}

void uniform_range_begin_end (int count, int &begin, int &end, const thread_info &thr_info)
{
  int thr_id = thr_info.get_thread_id ();
  int thr_total = thr_info.get_threads_total ();

  begin = thr_id * count / thr_total;
  end = (thr_id + 1) * count / thr_total;
}

counting_range_t<int> uniform_range_begin_end (int count, const thread_info &thr_info)
{
  int begin, end;
  uniform_range_begin_end (count, begin, end, thr_info);

  return range (begin, end);
}
