#ifndef ND_PROJECT_H
#define ND_PROJECT_H

#include <memory>
#include <vector>

class waitable_request;
class thread_group_class;
class task;

class events_queue;
class waker;
class message_t;

class manager;

class nd_manager
{
private:
  int m_total_threads = 0;

  std::unique_ptr<waitable_request> m_task_queue;
  std::unique_ptr<thread_group_class> m_task_threads;

  std::unique_ptr<events_queue> m_events_queue;
  std::unique_ptr<waker> m_waker;

  manager *m_manager = nullptr;

public:
  nd_manager ();
  ~nd_manager ();

  int get_total_threads () const;

  void set_manager (manager *mgr);

  events_queue *get_events_queue ();
  void process_events ();

  bool add_task (task *f);
  void create_kernel_threads (int total_threads);

  void process_print_log (message_t to_print);
};

#endif // ND_PROJECT_H