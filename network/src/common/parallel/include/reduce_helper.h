#ifndef REDUCE_HELPER_H
#define REDUCE_HELPER_H

enum class reduce_op_t
{
  sum,
  min,
  max,
};

template<typename DataType, reduce_op_t>
struct reduce_helper;

template<typename DataType>
struct reduce_helper<DataType, reduce_op_t::sum>
{
  using DataTypePtr = DataType *;

  static void reduce_nolock (DataTypePtr &res, DataType *data, size_t count)
  {
    if (!res)
      res = data;
    else
      for (size_t i = 0; i < count; i++)
        res[i] += data[i];
  }
};

template<typename DataType>
struct reduce_helper<DataType, reduce_op_t::max>
{
  using DataTypePtr = DataType *;

  static void reduce_nolock (DataTypePtr &res, DataType *data, size_t /*count*/)
  {
    if (!res)
      res = data;
    else
      if (*res < *data)
        res = data;
  }
};

template<typename DataType>
struct reduce_helper<DataType, reduce_op_t::min>
{
  using DataTypePtr = DataType *;

  static void reduce_nolock (DataTypePtr &res, DataType *data, size_t /*count*/)
  {
    if (!res)
      res = data;
    else
      if (*res > *data)
        res = data;
  }
};

#endif // REDUCE_HELPER_H
