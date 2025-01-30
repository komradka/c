#include "signal.hpp"

#pragma once

class Slots
{
public:
    Slots () = default;
    ~Slots ()
    {
      for (auto *conn = m_first_connection; conn; )
        {
          auto *next_conn = conn->m_next_slot;
          conn->destroy_connection ();
          conn = next_conn;
        }
    }

    void connection_destroyed (abstract_connection *conn)
    {
      if (conn == m_first_connection)
        m_first_connection = conn->m_next_slot;

      if (conn == m_last_connection)
        m_last_connection = conn->m_prev_slot;
    }

    template <typename... Args, typename FUNC>
    void connect_to(Signal<Args...> &s, FUNC &&f)
    {
       auto *conn = s.Connect(std::forward<FUNC>(f));
       conn->m_slot = this;

       if (!m_first_connection)
         m_first_connection = conn;

       conn->m_prev_slot = m_last_connection;

       if (m_last_connection)
         m_last_connection->m_next_slot = conn;

       m_last_connection = conn;
    }

public:
    abstract_connection *m_first_connection = nullptr, *m_last_connection = nullptr;
};
