#ifndef CALCULATE_TASK_H
#define CALCULATE_TASK_H

#include "task.h"

class graph;

class CALCULATE_task : public task
{
public:
  CALCULATE_task (nd_manager &manager) : task (manager) {}
  const char *get_task_name () const override;
  nd_manager *create_comanager (nd_manager &manager) const override;
  error verify_before_run () const override;
  error multithread_run (const thread_info &thr_info) override;
};

#endif // CALCULATE_TASK_H
