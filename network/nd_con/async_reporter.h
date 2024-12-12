#include <string>
#include <vector>
#include <memory>
#include <cstdarg>

#include "gui/gui_feedback/events.h"

#pragma once

std::string rep_sprintf (const char *format, va_list va);

enum class msg_sections
{
  task,
  service,

  UNDEFINED,
};

#define RUN_SECTION msg_sections::task
#define TOP_SECTION msg_sections::service

enum class msg_types
{
  message,
  error,
  warning,

  EMPTY,
};

#define MESSAGE msg_types::message
#define ERROR msg_types::error
#define WARNING msg_types::warning

struct message_t
{
  msg_types type = msg_types::EMPTY;
  msg_sections section = msg_sections::UNDEFINED;

  std::string message = "";
};

class nd_manager;
class print_log_event : public event_t
{
private:
  nd_manager &m_project;
  message_t m_message;

public:
  print_log_event (nd_manager &project, message_t message);
  void process () override;
};

class async_reporter_t
{
private:
  nd_manager &m_project;

public:
  async_reporter_t (nd_manager &project);
  ~async_reporter_t () = default;

  void add_prefixes (message_t &msg) const;
  void print (msg_types type, msg_sections section, const char *format, ...);

private:
  void print_message (message_t message);
};