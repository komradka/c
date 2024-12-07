#ifndef WAITABLE_REQUEST_H
#define WAITABLE_REQUEST_H

#include <memory>

class nd_mutex_t;
class nd_cond_var_t;

class task;

class waitable_request
{
private:
  std::unique_ptr<nd_mutex_t> m_mutex;
  std::unique_ptr<nd_cond_var_t> m_cond_var;

  task *m_value = nullptr;
  bool m_exit = false;

public:
  waitable_request ();
  ~waitable_request ();

  bool is_free () const;

  bool request_value (task *value);
  void request_exit ();
  task *wait ();
};

#endif // WAITABLE_REQUEST_H
