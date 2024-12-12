#include <memory>

#include "../../error.hpp"

#pragma once

class async_reporter_t;
class thread_info;

class task
{
protected:
  std::unique_ptr<async_reporter_t> m_reporter;

public:
  void set_reporter (async_reporter_t *reporter);
  void run (const thread_info &thr_info);

  virtual const char *get_task_name () const = 0;
  virtual error multithread_run (const thread_info &thr_info) = 0;
};

