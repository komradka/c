#include <memory>

#include "../../error.hpp"

#pragma once

class async_reporter_t;
class thread_info;
class nd_manager;

class task
{
protected:
  std::unique_ptr<async_reporter_t> m_reporter;

public:
  void from_manager (nd_manager &project);
  void set_reporter (async_reporter_t *reporter);
  void run (const thread_info &thr_info);

  virtual const char *get_task_name () const = 0;
  virtual nd_manager *create_comanager (nd_manager &manager) const = 0;
  virtual error verify_before_run () const;
  virtual error multithread_run (const thread_info &thr_info) = 0;

protected:
  std::unique_ptr<nd_manager> m_manager;
};

