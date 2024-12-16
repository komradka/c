#include <functional>

#pragma once

class abstract_connection;
class abstract_signal
{
public:
  void connection_destroyed (abstract_connection *conn);

public:
  abstract_connection *m_first_connection = nullptr, *m_last_connection = nullptr;
};

class Slots;
class abstract_connection
{
public:
  void destroy_connection ();

public:
  abstract_signal *m_signal = nullptr;
  Slots *m_slot = nullptr;

  abstract_connection *m_next_signal = nullptr, *m_prev_signal = nullptr;
  abstract_connection *m_next_slot = nullptr, *m_prev_slot = nullptr;
};

template<typename Func_t>
class connection : public abstract_connection
{
public:
  Func_t m_func;
};

template<typename ... Args>
class Signal : public abstract_signal
{
  using func_t = std::function<void (Args ...)>;

public:
    Signal() = default;

    ~Signal ()
    {
      for (auto *conn = m_first_connection; conn; )
        {
          auto *next_conn = conn->m_next_signal;
          conn->destroy_connection ();
          conn = next_conn;
        }
    }

    abstract_connection *Connect(func_t &&f)
    {
      auto *conn = new connection<func_t>;
      conn->m_signal = this;
      conn->m_func = std::move (f);

      if (!m_first_connection)
        m_first_connection = conn;

      conn->m_prev_signal = m_last_connection;

      if (m_last_connection)
        m_last_connection->m_next_signal = conn;

      m_last_connection = conn;

      return conn;
    }

    void Emit(Args&&... args)
    {
      for (auto *conn = m_first_connection; conn; conn = conn->m_next_signal)
        static_cast<connection<func_t> *> (conn)->m_func (std::forward<Args>(args)...);
    }


    void operator()(Args&& ... args)
    {
      Emit (std::forward<Args>(args)...);
    }
};
