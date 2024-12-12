#include <memory>
#include <vector>

#include "../project_window/nd_window.hpp"
#include "../kernel/topology.hpp"

class waitable_request;
class thread_group_class;
class task;

class events_queue;
class waker;
class message_t;

class manager;

#pragma once

class nd_manager
{
private:
  int m_total_threads = 0;

  std::unique_ptr<waitable_request> m_task_queue;
  std::unique_ptr<thread_group_class> m_task_threads;

  std::unique_ptr<events_queue> m_events_queue;
  std::unique_ptr<waker> m_waker;

  manager *m_manager = nullptr;

  nd_main_window *nd_window;
  graph *network_topology;
  reporter *rep;

  std::string name;

public:
  nd_manager(std::string name);
  ~nd_manager();

  int get_total_threads() const;

  void set_manager(manager *mgr);

  events_queue *get_events_queue();
  void process_events();

  bool add_task(task *f);
  void create_kernel_threads(int total_threads);

  void process_print_log(message_t to_print);

  void create_topology();

  error create_network_object(const std::string type, std::string data_file = "", vertex **v = nullptr);
  error add_link(object_id f, object_id s, link **l = nullptr);
  object_id get_object_by_name(std::string name);
  std::pair<object_id, object_id> get_connected_object(link_id id);
  void delete_link(link_id id);
  void delete_object(object_id id, std::vector<object_id> *connected_objects = nullptr, std::vector<link_id> *object_links = nullptr);
};
