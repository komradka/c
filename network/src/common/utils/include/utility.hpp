#include <string>
#include <type_traits>
#include <optional>

#pragma once

template <auto Start, auto End, auto Inc = 1, typename F>
constexpr void constexpr_for(F &&f)
{
    static_assert(Inc != 0);
    if constexpr ((Inc > 0 && Start < End) || (Inc < 0 && End < Start))
    {
        f(std::integral_constant<decltype(Start), Start>());
        constexpr_for<Start + Inc, End, Inc>(f);
    }
}

template <typename T>
struct is_optional : public std::false_type
{
};

template <typename T>
struct is_optional<std::optional<T>> : public std::true_type
{
};

template<typename T>
int isize (const T &something)
{
  return static_cast<int> (something.size ());
}

std::string get_name_for_gui(int e);

std::string get_name_for_gui(double e);

std::string get_name_for_gui(bool e);

std::string get_name(int e);

std::string get_name(double e);

std::string get_name(bool e);

template<typename Int>
class counting_iterator_t
{
private:
  Int m_value;

public:
  counting_iterator_t (Int value) : m_value (value) {}

  Int operator* () { return m_value; }
  counting_iterator_t operator++ () { return counting_iterator_t{m_value++}; }

  friend bool operator== (const counting_iterator_t &lhs, const counting_iterator_t &rhs) { return lhs.m_value == rhs.m_value; }
  friend bool operator!= (const counting_iterator_t &lhs, const counting_iterator_t &rhs) { return lhs.m_value != rhs.m_value; }
  friend bool operator< (const counting_iterator_t &lhs, const counting_iterator_t &rhs) { return lhs.m_value < rhs.m_value; }
  friend bool operator> (const counting_iterator_t &lhs, const counting_iterator_t &rhs) { return lhs.m_value > rhs.m_value; }
  friend bool operator<= (const counting_iterator_t &lhs, const counting_iterator_t &rhs) { return lhs.m_value <= rhs.m_value; }
  friend bool operator>= (const counting_iterator_t &lhs, const counting_iterator_t &rhs) { return lhs.m_value >= rhs.m_value; }
};

template<typename Int>
class counting_range_t
{
private:
  Int m_begin, m_end;

public:
  using iterator = counting_iterator_t<Int>;

  counting_range_t (Int begin, Int end) : m_begin (begin), m_end (end) {}

  iterator begin () const { return iterator{m_begin}; }
  iterator end () const { return iterator{m_end}; }
};

template<typename Int>
counting_range_t<Int> range (Int begin, Int end)
{
  return {begin, end};
}
