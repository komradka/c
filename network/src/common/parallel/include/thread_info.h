#include <stdio.h>

#include "error.hpp"
#include "utility.hpp"
#include "pthread_utils.h"
#include "reduce_helper.h"

#pragma once

constexpr unsigned int MAIN_THREAD_ID = 0;

class thread_info
{
private:
  int m_thread_id;
  int m_threads_total;

public:
  thread_info (int thread_id, int threads_total);
  ~thread_info () = default;

  bool is_main_thread () const { return m_thread_id == 0; }
  bool is_multithread () const { return m_threads_total > 1; }

  int get_thread_id () const { return m_thread_id; }
  int get_threads_total () const { return m_threads_total; }

  void barrier () const;

  template<typename DataType>
  void bcast (DataType *data, size_t count, unsigned int root) const;

  template<typename DataType>
  void bcast (DataType &data, unsigned int root) const;

  template<typename DataType>
  void allreduce_sum (DataType *data, size_t count) const;

  template<typename DataType>
  void allreduce_sum (DataType &data) const;

  template<typename DataType>
  void allreduce_max (DataType *data, size_t count) const;

  template<typename DataType>
  void allreduce_max (DataType &data) const;

  template<typename DataType>
  void allreduce_min (DataType *data, size_t count) const;

  template<typename DataType>
  void allreduce_min (DataType &data) const;

  error reduce_error (error &err) const;

private:
  template<typename DataType>
  void share_data (DataType *res, DataType *data, size_t count) const
  {
    if (res != data)
      for (size_t i = 0; i < count; i++)
        data[i] = res[i];
  }

  template<typename DataType, reduce_op_t OP>
  void reduce (DataType *data, size_t count) const
  {
    static mutex_t mutex;
    static cond_var_t cond_in, cond_out;
    static DataType *res = nullptr;
    static int in = 0, out = 0;

    mutex.lock ();

    in++;
    reduce_helper<DataType, OP>::reduce_nolock (res, data, count);

    if (in >= get_threads_total ())
      {
        out = 0;
        cond_in.wake_all ();
      }
    else
      while (in < get_threads_total ())
        cond_in.wait (&mutex);

    out++;
    share_data (res, data, count);

    if (out >= get_threads_total ())
      {
        in = 0;
        res = nullptr;
        cond_out.wake_all ();
      }
    else
      while (out < get_threads_total ())
        cond_out.wait (&mutex);

    mutex.unlock ();
  }
};

void uniform_range_begin_end (int count, int &begin, int &end, const thread_info &thr_info);
counting_range_t<int> uniform_range_begin_end (int count, const thread_info &thr_info);

template<typename DataType>
void thread_info::bcast (DataType *data, size_t count, unsigned int root) const
{
  if (!is_multithread ())
    return;

  static DataType *res = nullptr;

  if (get_thread_id () == root)
    res = data;
  barrier ();

  share_data (res, data, count);
  barrier ();
}

template<typename DataType>
void thread_info::bcast (DataType &data, unsigned int root) const
{
  bcast (&data, 1, root);
}

template<typename DataType>
void thread_info::allreduce_sum (DataType *data, size_t count) const
{
  if (is_multithread ())
    reduce<DataType, reduce_op_t::sum> (data, count);
}

template<typename DataType>
void thread_info::allreduce_sum (DataType &data) const
{
  allreduce_sum (&data, 1);
}

template<typename DataType>
void thread_info::allreduce_max (DataType *data, size_t count) const
{
  if (is_multithread ())
    reduce<DataType, reduce_op_t::max> (data, count);
}

template<typename DataType>
void thread_info::allreduce_max (DataType &data) const
{
  allreduce_max (&data, 1);
}

template<typename DataType>
void thread_info::allreduce_min (DataType *data, size_t count) const
{
  if (is_multithread ())
    reduce<DataType, reduce_op_t::min> (data, count);
}

template<typename DataType>
void thread_info::allreduce_min (DataType &data) const
{
  allreduce_min (&data, 1);
}

