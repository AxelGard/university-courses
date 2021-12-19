#include <ostream>
#include <iterator>
#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <array>
#include <string>

template <typename T>
void print(std::ostream& os, T const& data);

namespace details
{
    /* specialization for std::string since we don't want to
       print it as a container (even though it is one)       */
    void print(std::ostream& os, std::string const& str, int)
    {
      os << str;
    }

    void print(std::ostream& os, char const* str, int)
    {
      os << str;
    }
    
    template <std::size_t N>
    void print(std::ostream& os, char const (&str)[N], int)
    {
      os << str;
    }

    /* general printing functionality for printing an iterator
       range, used by the container and std::array overloads */
    template <typename It>
    void print_range(std::ostream& os, It start, It last)
    {
      os << '{';
      if (start != last)
      {
        ::print(os, *start++);
        while (start != last)
        {
          os << ", ";
          ::print(os, *start++);
        }
      }
      os << '}';
    }

    /* std::array is both a tuple-type and a container so to remove
       ambiguities we must make a specific overload for it */
    template <typename T, std::size_t N>
    void print(std::ostream& os, std::array<T, N> const& data, int)
    {
      print_range(os, std::begin(data), std::end(data));
    }

    /* print anything that is a container.
       Here we check for the existence of iterators to deduce
       that it is a container */
    template <typename T>
    auto print(std::ostream& os, T const& data, int)
      -> decltype(std::begin(data), std::declval<void>())
    {
      print_range(os, std::begin(data), std::end(data));
    }

    template <typename T, int N>
    void print_tuple(std::ostream& os, T const& data, std::integer_sequence<int, N>)
    {
      ::print(os, std::get<N>(data));
    }
    
    template <typename T, int N, int... Ns>
    void print_tuple(std::ostream& os, T const& data, std::integer_sequence<int, N, Ns...>)
    {
      ::print(os, std::get<N>(data));
      os << ' ';
      print_tuple(os, data, std::integer_sequence<int, Ns...>{});
    }
    
    /* print tuple-types (std::pair, std::tuple and std::array).
       We are using std::integer_sequence to generate a sequence of integers.
       Thanks to this we can iterate over the tuple during compile-time with
       variadic recursion.

       We are using std::tuple_size to deduce how many elements are stored
       in the tuple, and we are accessing elements with std::get
    */
    template <typename T>
    auto print(std::ostream& os, T const& data, int)
      -> decltype(std::get<0>(data), std::tuple_size<T>::value, std::declval<void>())
    {
      os << '(';
      print_tuple(os, data, std::make_integer_sequence<int, std::tuple_size<T>::value>{});
      os << ')';
    }

    /* Sink function, note the last parameter; this is to make sure that it has
       lower priority during overload resolution */
    template <typename T>
    void print(std::ostream& os, T const& data, double)
    {
      os << data;
    }
}

/* General print function which will call appropriate helper functions
   based of T */
template <typename T>
void print(std::ostream& os, T const& data)
{
  details::print(os, data, 0);
}

int main()
{
  print(std::cout, 5);
  std::cout << std::endl;
  
  std::vector<int> v {1, 2, 3};
  print(std::cout, v);
  std::cout << std::endl;

  std::map<int, int> m { {1, 1}, {2, 2}, {3, 3} };
  print(std::cout, m);
  std::cout << std::endl;

  std::tuple<int, double> t { 5, 3.14 };
  print(std::cout, t);
  std::cout << std::endl;

  std::string s[] { "hello", "world" };
  print(std::cout, s);
  std::cout << std::endl;
  
  std::array<std::vector<std::string>, 2> a { std::vector<std::string>{ "ab", "c" }, std::vector<std::string>{"def", "g", "hi"} };
  print(std::cout, a);
  std::cout << std::endl;

  char const* str {"SFINAE"};
  print(std::cout, str);
  std::cout << std::endl;

  print(std::cout, "string literal");
  std::cout << std::endl;
}