#include <vector>
#include <cassert>
#include <forward_list>
#include <list>

// binary search
int binary_search(int first, int last, T const& value)
{
  int error{last};
  if (first != last)
  {
    while (first < last)
    {
      int pivot{first + (last - first) / 2};
      if (/* container[pivot] < value */)
        first = pivot+1;
      else if (/* container[pivot] > value */)
        last = pivot;
      else
        return pivot;
    }
  }
  return error;
}

// linear search
int linear_search(int first, int last, T const& value)
{
  for (int pivot{first}; pivot != last; ++pivot)
  {
    if (/* container[pivot] == value */)
      return pivot;
    else if (/* container[pivot] > value */)
      break;
  }
  return last;
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