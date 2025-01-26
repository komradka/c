#include <pthread.h>
#include <string>
#include <vector>
#include <memory>

#include "thread_info.h"

#pragma once

class waitable_request;
class task;

void *start_thread_function (void *Args);

class thread_group_class;

class thread_class
{
private:
  pthread_t m_thread;
  std::string m_thread_name;
  thread_info m_thr_info;
  thread_group_class *m_group;

public:
  thread_class (const std::string &thread_name, thread_info thr_info, thread_group_class *group);
  ~thread_class ();

  std::string get_thread_name () const;

  int start ();
  int join ();
  void run ();
};

class thread_group_class
{
private:
  std::string m_group_name;
  std::vector<std::unique_ptr<thread_class>> m_threads;

  waitable_request *m_task_queue;

  task *m_current_task = nullptr;

public:
  thread_group_class (const std::string &group_name, int total_threads, waitable_request *task_queue);
  ~thread_group_class ();

  void start ();
  void join ();
  void run (const thread_info &thr_info);
};
