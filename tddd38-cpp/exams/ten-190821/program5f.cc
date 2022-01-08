#include <iterator>
#include <vector>
#include <cassert>
#include <forward_list>
#include <list>

namespace details
{
    template <typename It, typename T>
    It search(It first, It last, T const& value, std::random_access_iterator_tag)
    {
      It error{last};
      if (first != last)
      {
        while (first < last)
        {
          It pivot{first + (last - first) / 2};
          if (*pivot < value)
            first = std::next(pivot);
          else if (*pivot > value)
            last = pivot;
          else
            return pivot;
        }
      }
      return error;
    }

    template <typename It, typename T>
    It search(It first, It last, T const& value, std::forward_iterator_tag)
    {
      for (It pivot{first}; pivot != last; pivot = std::next(pivot))
      {
        if (*pivot == value)
          return pivot;
        else if (*pivot > value)
          break;
      }
      return last;
    }
}

template <typename It>
It search(It first, It last, typename std::iterator_traits<It>::value_type const& value)
{
  return details::search(first, last, value, typename std::iterator_traits<It>::iterator_category{});
}

int main()
{
  {
    std::vector<int> v {1,2,3,4,5};
    std::vector<int> e {};
    std::vector<int> o {1};
    assert((*search(o.begin(), o.end(), 1) == 1));
    for (auto i : v)
      assert((*search(v.begin(), v.end(), i) == i));
    assert((search(v.begin(), v.end(), 6) == v.end()));
    assert((search(e.begin(), e.end(), 0) == e.end()));
    assert((search(o.begin(), o.end(), 0) == o.end()));
  }
  {
    std::list<int> v {1,2,3,4,5};
    std::list<int> e {};
    std::list<int> o {1};
    assert((*search(o.begin(), o.end(), 1) == 1));
    for (auto i : v)
      assert((*search(v.begin(), v.end(), i) == i));
    assert((search(v.begin(), v.end(), 6) == v.end()));
    assert((search(e.begin(), e.end(), 0) == e.end()));
    assert((search(o.begin(), o.end(), 0) == o.end()));
  }
  {
    std::forward_list<int> v {1,2,3,4,5};
    std::forward_list<int> e {};
    std::forward_list<int> o {1};
    assert((*search(o.begin(), o.end(), 1) == 1));
    for (auto i : v)
      assert((*search(v.begin(), v.end(), i) == i));
    assert((search(v.begin(), v.end(), 6) == v.end()));
    assert((search(e.begin(), e.end(), 0) == e.end()));
    assert((search(o.begin(), o.end(), 0) == o.end()));
  }
}