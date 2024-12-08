#ifndef ND_MIXIN_H
#define ND_MIXIN_H

#include "nd_abstract_data.h"

template<typename Resource, typename Vertex>
class nd_mixin : public nd_abstract_data
{
public:

  Resource *get_resource () { return m_resource; }
  Vertex *get_vertex () { return m_vertex; }

private:
  Resource *m_resource = nullptr;
  Vertex *m_vertex = nullptr;
};

#endif // ND_MIXIN_H
