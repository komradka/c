#include "nd_manager.h"

#include "nd/tasks/task.h"
#include "nd/queue/thread_class.h"
#include "nd/queue/waitable_request.h"

#include "events.h"

#include "async_reporter.h"

#include "manager.hpp"

nd_manager::nd_manager(std::string name)
{
  wf = new workflow();
  this->name = name;
  nd_window = new nd_main_window(name);
  nd_window->set_pm(this);
  nd_window->set_workflow();
  nd_window->show();
  create_reporter(nd_window->get_wrep());
  nd_window->set_reporter(rep);
  create_topology();
  settings = new settings_dialog();

  m_events_queue = std::make_unique<events_queue>();
  m_waker = std::make_unique<waker>(*this);
  QObject::connect(m_events_queue.get(), &events_queue::received_event, m_waker.get(), &waker::wake_up, Qt::QueuedConnection);
  m_slots.connect_to(nd_window->wf->start_calculation_signal, [&]()
                     { wf->calculate(rep, nd_window->wf); });

  rep->print (message_type::MESSAGE, message_source::ND, message_category::ALL, "Project %s created", name.c_str ());
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
    reporter->print(message_type::ERR, TOP_SECTION, "Calulation \"%s\" cannot be started.\nError: %s", f->get_task_name(), err.description_cstr());
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
  case message_type::ERR:
    rep->print(message_type::ERR, message_source::ND, message_category::LOG, to_print.message.c_str ());
    break;
  case message_type::MESSAGE:
    rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG, to_print.message.c_str ());
    break;
  case message_type::WARNING:
    rep->print(message_type::WARNING, message_source::ND, message_category::LOG, to_print.message.c_str ());
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
    rep->print(message_type::ERR, message_source::ND, message_category::LOG, "%s in file %s", ret.description_cstr(), data_file.c_str());
    return ret;
  }

  return error(OK);
}

error nd_manager::add_link(object_id f, object_id s, link **l)
{
  *l = network_topology->create_link(f, s);

  rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG,
             "Link between %s and %s successfully created", network_topology->get_object_name(f).c_str (), network_topology->get_object_name(s).c_str ());

  return error(OK);
}

void nd_manager::copy_results(const std::map<object_id, phys_q> &object_res)
{
  nd_window->copy_results(object_res);
}

object_id nd_manager::get_object_by_name(std::string name)
{
  std::optional<object_id> obj = network_topology->get_object_by_name(name);

  if (!obj)
  {
    rep->print(message_type::ERR, message_source::ND, message_category::LOG, "Cannot find object %s", name.c_str());
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

  rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG,
             "Link between %s and %s successfully deleted", network_topology->get_object_name(connected_objs.first).c_str (), network_topology->get_object_name(connected_objs.second).c_str ());

  network_topology->delete_link(id);
}

void nd_manager::delete_object(object_id id, std::vector<object_id> *connected_objects, std::vector<link_id> *object_links)
{
  std::string name = network_topology->get_object_name(id);

  rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG, "Object %s deleted", name.c_str());

  *object_links = network_topology->get_object_links(id);
  *connected_objects = network_topology->get_neighbors(id);

  network_topology->delete_object(id);
}

void nd_manager::show_settings()
{
  settings->show();
}

pvt_manager *nd_manager::create_fluid(std::string name)
{
  create_fluid_signal.Emit(name);
  return network_PVT.back();
}

void nd_manager::save_project(std::string res_name)
{
  rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG, "Saving...");

  writer *w = new writer(name, rep, network_topology, nd_window->get_gui_manager(), settings, &network_PVT, fluid_id);

  error ret = w->write_network_to_file(results_count, QString::fromStdString(res_name));
  if (!ret.is_ok())
  {
    rep->print(message_type::ERR, message_source::ND, message_category::LOG, "%s", ret.description_cstr());
    delete w;
    return;
  }

  rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG, "Project saved");
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
  auto project_name = res.res_name;
  rep->print(message_type::MESSAGE, message_source::ND, message_category::LOG, "Load %s", project_name.c_str());

  error ret = file_reader.read_data(res, nd_window->get_gui_manager(), settings);

  if (!ret.is_ok())
  {
    rep->print(message_type::ERR, message_source::ND, message_category::LOG, "%s", ret.description_cstr());
  }
  else
  {
    network_topology->update_active_objects();
  }
}

void nd_manager::set_fluid(int id)
{
  fluid = network_PVT[id]->get_widget();
  fluid_id = id;
}

void nd_manager::create_reporter(report_widget *wrep)
{
  rep = new report_system();

  rep->add_reporter([](message_type type, message_source source, message_category category) { return true; },
                    wrep);
}

fluid_props *nd_manager::get_fluid()
{
    fluid_props *fp = nullptr;
    if (fluid)
        fp = new fluid_props(fluid);

    return fp;
}
