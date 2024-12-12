#include <utility>

#include "thread_info.h"

#pragma once

template<class Type>
class thread_shared_ptr
{
private:
  Type *m_ptr = nullptr;

  const thread_info *m_thr_info = nullptr;

public:
  thread_shared_ptr () = default;

  template<typename ...Args>
  thread_shared_ptr (const thread_info &thr_info, Args&& ...args)
  {
    m_thr_info = &thr_info;
    allocate (thr_info, std::forward<Args> (args)...);
  }

  thread_shared_ptr (const thread_info &thr_info, Type *ptr)
  {
    m_thr_info = &thr_info;
    m_ptr = ptr;
    ptr = nullptr;
  }

  ~thread_shared_ptr ()
  {
    m_thr_info->barrier ();
    if (m_thr_info->is_main_thread ())
      delete m_ptr;
    m_thr_info->barrier ();
  }

  Type *operator -> ()
  {
    return get ();
  }

  template<typename ...Args>
  bool make_thr_shared (const thread_info &thr_info, Args&& ...args)
  {
    m_thr_info = &thr_info;
    return allocate (std::forward<Args> (args)...);
  }

  Type *release ()
  {
    Type *ptr = m_ptr;
    m_thr_info->barrier ();
    if (m_thr_info->is_main_thread ())
      m_ptr = nullptr;
    m_thr_info->barrier ();
    return ptr;
  }

  void reset (Type *ptr)
  {
    m_thr_info->barrier ();
    if (m_thr_info->is_main_thread ())
      {
        if (m_ptr)
          delete m_ptr;
        m_ptr = ptr;
      }
    m_thr_info->barrier ();
  }

  Type *get () { return m_ptr; }

private:
  template<typename ...Args>
  bool allocate (Args&& ...args)
  {
    if (m_thr_info->is_main_thread ())
      {
        m_ptr = new Type (std::forward<Args> (args)...);
      }
    m_thr_info->barrier ();

    m_thr_info->bcast<Type> (&m_ptr);
    return true;
  }
};

template<class Type>
class thread_shared_ptr<Type[]>
{
private:
  Type *m_ptr = nullptr;

  const thread_info *m_thr_info = nullptr;

public:
  thread_shared_ptr () = default;

  thread_shared_ptr (const thread_info &thr_info, size_t n)
  {
    m_thr_info = &thr_info;
    allocate (n);
  }

  thread_shared_ptr (const thread_info &thr_info, Type *ptr)
  {
    m_thr_info = &thr_info;
    m_ptr = ptr;
    ptr = nullptr;
  }

  ~thread_shared_ptr ()
  {
    m_thr_info->barrier ();
    if (m_thr_info->is_main_thread ())
      {
        if (m_ptr)
          delete [] m_ptr;
      }
    m_thr_info->barrier ();
  }

  Type &operator [] (size_t index)
  {
    return m_ptr[index];
  }

  bool make_thr_shared (const thread_info &thr_info, size_t n)
  {
    m_thr_info = &thr_info;
    return allocate (thr_info, n);
  }

  Type *release ()
  {
    Type *ptr = m_ptr;
    m_thr_info->barrier ();
    if (m_thr_info->is_main_thread ())
      m_ptr = nullptr;
    m_thr_info->barrier ();
    return ptr;
  }

  void reset (Type *ptr)
  {
    m_thr_info->barrier ();
    if (m_thr_info->is_main_thread ())
      {
        if (m_ptr)
          delete [] m_ptr;
      }
    m_ptr = ptr;
    m_thr_info->barrier ();
  }

  Type *get () { return m_ptr; }

private:
  bool allocate (size_t n)
  {
    if (m_thr_info->is_main_thread ())
      {
        m_ptr = new Type [n];
      }
    m_thr_info->barrier ();

    m_thr_info->bcast (&m_ptr);
    return true;
  }

};