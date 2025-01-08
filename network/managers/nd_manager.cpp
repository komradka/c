#include "nd_manager.h"

#include "../nd_con/tasks/task.h"
#include "../parallel/thread_class.h"
#include "../parallel/waitable_request.h"

#include "../gui/gui_feedback/events.h"

#include "../nd_con/async_reporter.h"

#include "../manager.hpp"

nd_manager::nd_manager(std::string name)
{
  wf = new workflow();
  this->name = name;
  nd_window = new nd_main_window(name);
  nd_window->set_pm(this);
  nd_window->set_workflow();
  nd_window->show();
  rep = nd_window->get_reporter();
  create_topology();
  settings = new settings_dialog();

  m_events_queue = std::make_unique<events_queue>();
  m_waker = std::make_unique<waker>(*this);
  QObject::connect(m_events_queue.get(), &events_queue::received_event, m_waker.get(), &waker::wake_up, Qt::QueuedConnection);
}

nd_manager::~nd_manager()
{
  m_task_queue->request_exit();
  m_task_threads->join();

  delete wf;
  delete nd_window;
  delete settings;
}

int nd_manager::get_total_threads() const
{
  return m_total_threads;
}

void nd_manager::set_manager(manager *mgr)
{
  m_manager = mgr;
}

events_queue *nd_manager::get_events_queue()
{
  return m_events_queue.get();
}

void nd_manager::process_events()
{
  while (auto event = m_events_queue->pop())
    event->process();
}

bool nd_manager::add_task(task *f)
{
  async_reporter_t *reporter = new async_reporter_t(*this);

  f->from_manager(*this);

  error err = f->verify_before_run();
  if (!err.is_ok())
  {
    reporter->print(ERROR, TOP_SECTION, "Calulation \"%s\" cannot be started.\nError: %s", f->get_task_name(), err.description_cstr());
    return false;
  }

  f->set_reporter(reporter);
  return m_task_queue->request_value(f);
}

void nd_manager::create_kernel_threads(int total_threads)
{
  m_total_threads = total_threads;

  m_task_queue = std::make_unique<waitable_request>();
  m_task_threads = std::make_unique<thread_group_class>("ND:", total_threads, m_task_queue.get());

  m_task_threads->start();
}

void nd_manager::process_print_log(message_t to_print)
{
  switch (to_print.type)
  {
  case msg_types::error:
    rep->print_error(to_print.message);
    break;
  case msg_types::message:
    rep->print_message(to_print.message);
    break;
  case msg_types::warning:
    rep->print_warning(to_print.message);
    break;
  case msg_types::EMPTY:
    break;
  }
}

void nd_manager::create_topology()
{
  network_topology = new graph(rep);
}

error nd_manager::create_network_object(const std::string type, std::string data_file, vertex **v)
{
  error ret = network_topology->make_object(type, data_file, v);
  if (!ret.is_ok())
  {
    rep->print_error(string(ret) + " in file " + data_file);
    return ret;
  }

  return error(OK);
}

error nd_manager::add_link(object_id f, object_id s, link **l)
{
  *l = network_topology->create_link(f, s);

  rep->print_message("Link between " + network_topology->get_object_name(f) + " and " + network_topology->get_object_name(s) + " successfully created");

  return error(OK);
}

void nd_manager::copy_results(const std::map<object_id, phys_q> &object_res)
{
  m_manager->copy_results(object_res);
}

object_id nd_manager::get_object_by_name(std::string name)
{
  std::optional<object_id> obj = network_topology->get_object_by_name(name);

  if (!obj)
  {
    rep->print_error("Cannot find object " + name);
    return {};
  }

  return obj.value();
}

std::pair<object_id, object_id> nd_manager::get_connected_object(link_id id)
{
  return network_topology->get_connected_object(id);
}

graph *nd_manager::get_network_topology()
{
  return network_topology;
}

settings_dialog *nd_manager::get_settings()
{
  return settings;
}

workflow *nd_manager::get_workflow()
{
  return wf;
}

void nd_manager::set_network_topology(graph *topology)
{
  network_topology = topology;
}

void nd_manager::set_settings(settings_dialog *settings)
{
  this->settings = settings;
}

void nd_manager::delete_link(link_id id)
{
  std::pair<object_id, object_id> connected_objs = network_topology->get_connected_object(id);

  rep->print_message("Link between " + network_topology->get_object_name(connected_objs.first) + " and " + network_topology->get_object_name(connected_objs.second) + " deleted");

  network_topology->delete_link(id);
}

void nd_manager::delete_object(object_id id, std::vector<object_id> *connected_objects, std::vector<link_id> *object_links)
{
  std::string name = network_topology->get_object_name(id);
  rep->print_message("Object " + name + " deleted");

  *object_links = network_topology->get_object_links(id);
  *connected_objects = network_topology->get_neighbors(id);

  network_topology->delete_object(id);
}

void nd_manager::show_settings()
{
  settings->show();
}

fluid_widget *nd_manager::create_fluid()
{
  fluid = new fluid_widget;
  return fluid;
}

void nd_manager::save_project(std::string res_name)
{
  rep->print_message("Saving...");

  writer *w = new writer(name, rep, network_topology, nd_window->get_gui_manager(), settings);

  error ret = w->write_network_to_file(results_count, QString::fromStdString(res_name));
  if (!ret.is_ok())
  {
    rep->print_error(ret);
    delete w;
    return;
  }

  rep->print_message("Project saved");
  results_count++;
  delete w;

  return;
}

void nd_manager::load_project_handler()
{
  std::vector<result_info> results;
  file_reader.read_project(name, results);

  results_count = results.size();
  nd_window->load_project(results);
}

void nd_manager::load_project(result_info &res)
{
  auto project_name = QString::fromStdString(res.res_name);
  rep->print_message("Loading - " + res.res_name);

  error ret = file_reader.read_data(res, nd_window->get_gui_manager(), settings);

  if (!ret.is_ok())
  {
    rep->print_error(ret);
  }
  else
  {
    network_topology->update_active_objects();
  }
}