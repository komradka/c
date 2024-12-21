#include <memory>
#include <vector>
#include <functional>

#include "../../error.hpp"
#include "gui/gui_feedback/events.h"

#pragma once

class async_reporter_t;
class thread_info;
class nd_manager;

class task
{
public:
  task (nd_manager &manager);

  void from_manager (nd_manager &manager);
  void set_reporter (async_reporter_t *reporter);
  void run (const thread_info &thr_info);
  void after_finish (nd_manager &manager) const;
  void do_after_finish_in_gui (const std::function<void (nd_manager &manager)> &func);

  virtual const char *get_task_name () const = 0;
  virtual nd_manager *create_comanager (nd_manager &manager) const = 0;
  virtual error verify_before_run () const;
  virtual error multithread_run (const thread_info &thr_info) = 0;

protected:
  std::unique_ptr<async_reporter_t> m_reporter;
  std::unique_ptr<nd_manager> m_comanager;
  std::vector<std::function<void (nd_manager &manager)>> m_do_after_finish_in_gui;

private:
  nd_manager &m_manager;
};

class task_done_event : public event_t
{
public:
  task_done_event (nd_manager &manager, task *f);
  void process () override;

private:
  nd_manager &m_manager;
  task *m_task = nullptr;
};

