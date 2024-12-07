#ifndef ND_EVENTS_H
#define ND_EVENTS_H

#include <list>
#include <memory>

#include "parallel/nd_pthread.h"

#include <QObject>

class event_t
{
public:
  virtual ~event_t () = default;
  virtual void process () = 0;
};

class events_queue : public QObject
{
  Q_OBJECT

private:
  std::list<std::unique_ptr<event_t>> m_events;

  std::unique_ptr<nd_mutex_t> m_mutex;

public:
  events_queue ();
  ~events_queue () = default;

  bool queue_empty () const;
  void push (std::unique_ptr<event_t> event);
  std::unique_ptr<event_t> pop ();

  Q_SIGNAL void received_event ();

};

class nd_manager;
class waker : public QObject
{
private:
  nd_manager &m_project;

public:
  waker (nd_manager &projec);
  ~waker () = default;

  void wake_up ();
};

#endif // ND_EVENTS_H
