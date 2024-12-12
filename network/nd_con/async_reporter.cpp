#include "async_reporter.h"

#include "../managers/nd_manager.h"
#include "parallel/pthread_utils.h"

std::string rep_sprintf (const char *format, va_list va)
{
  va_list va_for_length;
  va_copy (va_for_length, va);
  int length = vsnprintf (nullptr, 0, format, va_for_length);
  va_end (va_for_length);

  std::string result;
  result.assign (length, '\0');
  vsprintf (&result[0], format, va);

  return result;
}

print_log_event::print_log_event (nd_manager &project, message_t message)
  : m_project (project), m_message (message)
{
}

void print_log_event::process ()
{
  m_project.process_print_log (m_message);
}

async_reporter_t::async_reporter_t (nd_manager &project)
  : m_project (project)
{
}

void async_reporter_t::add_prefixes (message_t &msg) const
{
  switch (msg.section)
    {
    case msg_sections::task:
      msg.message = "    " + msg.message;
      break;
    case msg_sections::service:
      break;
    case msg_sections::UNDEFINED:
      break;
    }
}

void async_reporter_t::print (msg_types type, msg_sections section, const char *format, ...)
{
  va_list va;
  va_start (va, format);

  message_t msg = {type, section, rep_sprintf (format, va)};
  add_prefixes (msg);

  va_end (va);

  print_message (std::move (msg));
}

void async_reporter_t::print_message (message_t message)
{
  std::unique_ptr<print_log_event> feedback_event = std::make_unique<print_log_event> (m_project, message);
  m_project.get_events_queue ()->push (std::move (feedback_event));
}
