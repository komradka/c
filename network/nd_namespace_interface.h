#ifndef ND_NAMESPACE_INTERFACE_H
#define ND_NAMESPACE_INTERFACE_H

#include <map>
#include <vector>

class nd_abstract_data;
class nd_namespace_interface
{
public:
  nd_namespace_interface () = default;
  ~nd_namespace_interface () = default;

  nd_abstract_data *find_object (int unique_id);
  std::vector<int> get_all_unique_ids () const;
  std::vector<nd_abstract_data *> get_all_objects ();

  void add_object (nd_abstract_data *data);
  void delete_object (int unique_id);

private:
  int m_last_index = -1;
  std::map<int, nd_abstract_data *> m_unique_id_column;
};

#endif // ND_NAMESPACE_INTERFACE_H
