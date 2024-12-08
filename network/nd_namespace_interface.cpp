#include "nd_namespace_interface.h"

#include "nd_abstract_data.h"

nd_abstract_data *nd_namespace_interface::find_object (int unique_id)
{
  auto iter = m_unique_id_column.find (unique_id);
  if (iter == m_unique_id_column.end ())
    return nullptr;
  return iter->second;
}

std::vector<int> nd_namespace_interface::get_all_unique_ids () const
{
  std::vector<int> ids;
  for (const auto &pair : m_unique_id_column)
    ids.push_back (pair.first);
  return ids;
}

std::vector<nd_abstract_data *> nd_namespace_interface::get_all_objects ()
{
  std::vector<nd_abstract_data *> data;
  for (const auto &pair : m_unique_id_column)
    data.push_back (pair.second);
  return data;
}

void nd_namespace_interface::add_object (nd_abstract_data *data)
{
  m_last_index++;
  data->set_unique_id (m_last_index);
  m_unique_id_column.insert ({m_last_index, data});
}

void nd_namespace_interface::delete_object (int unique_id)
{
  auto iter = m_unique_id_column.find (unique_id);
  if (iter != m_unique_id_column.end ())
    m_unique_id_column.erase (iter);
}
