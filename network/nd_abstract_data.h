#ifndef ND_ABSTRACT_DATA_H
#define ND_ABSTRACT_DATA_H

#include "kernel/objects_type.hpp"

#include <QObject>

class nd_abstract_data : public QObject
{
  Q_OBJECT

public:

  virtual network_objects get_object_type () const = 0;

  nd_abstract_data () = default;
  ~nd_abstract_data () = default;

  int get_unique_id () const { return m_unique_id; }
  void set_unique_id (int unique_id) { m_unique_id = unique_id; }

signals:
  void data_changed ();

private:
  int m_unique_id = -1;
};

#endif // ND_ABSTRACT_DATA_H
