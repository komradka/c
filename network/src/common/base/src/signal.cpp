#include "signal.hpp"
#include "slot.hpp"

void abstract_signal::connection_destroyed (abstract_connection *conn)
{
  if (conn == m_first_connection)
    m_first_connection = conn->m_next_signal;

  if (conn == m_last_connection)
    m_last_connection = conn->m_prev_signal;
}

void abstract_connection::destroy_connection ()
{
  auto *prev_signal = m_prev_signal;
  auto *next_signal = m_next_signal;

  if (prev_signal)
    prev_signal->m_next_signal = next_signal;
  if (next_signal)
    next_signal->m_prev_signal = prev_signal;

  auto *prev_slot = m_prev_slot;
  auto *next_slot = m_next_slot;

  if (prev_slot)
    prev_slot->m_next_slot = next_slot;
  if (next_slot)
    next_slot->m_prev_slot = prev_slot;

  m_signal->connection_destroyed (this);
  m_slot->connection_destroyed (this);

  delete this;
}
