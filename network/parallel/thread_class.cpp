#include <pthread.h>
#include "thread_class.h"
#include "waitable_request.h"

#include "con/tasks/task.h"

void *start_thread_function (void *Args)
{
  thread_class *thread = static_cast<thread_class *> (Args);
  pthread_setname_np (pthread_self (), thread->get_thread_name ().c_str ());

  thread->run ();

  return 0;
}

thread_class::thread_class (const std::string &thread_name, thread_info thr_info, thread_group_class *group)
  : m_thread_name (thread_name), m_thr_info (thr_info), m_group (group)
{
}

thread_class::~thread_class ()
{
}

std::string thread_class::get_thread_name () const
{
  return m_thread_name;
}

int thread_class::start ()
{
  return pthread_create (&m_thread, 0, start_thread_function, this);
}

int thread_class::join ()
{
  return pthread_join (m_thread, 0);
}

void thread_class::run ()
{
  m_group->run (m_thr_info);
}

thread_group_class::thread_group_class (const std::string &group_name, int total_threads, waitable_request *queue)
  : m_group_name (group_name), m_threads (total_threads), m_task_queue (queue)
{
}

thread_group_class::~thread_group_class ()
{
}

void thread_group_class::start ()
{
  int total_threads = static_cast<int> (m_threads.size ());
  for (int thread_num = 0; thread_num < total_threads; thread_num++)
    m_threads[thread_num] = std::make_unique<thread_class> (m_group_name + std::to_string (thread_num), thread_info{thread_num, total_threads}, this);

  for (int thread_num = 0; thread_num < total_threads; thread_num++)
    m_threads[thread_num]->start ();
}

void thread_group_class::join ()
{
  int total_threads = static_cast<int> (m_threads.size ());
  for (int thread_num = 0; thread_num < total_threads; thread_num++)
    m_threads[thread_num]->join ();
}

void thread_group_class::run (const thread_info &thr_info)
{
  while (true)
    {
      if (thr_info.is_main_thread ())
        m_current_task = m_task_queue->wait ();

      thr_info.barrier ();

      if (!m_current_task)
        break;

      m_current_task->run (thr_info);
    }
}
