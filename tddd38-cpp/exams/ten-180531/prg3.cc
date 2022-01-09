#include <list>
#include <vector>
#include <cassert>

/* Here is a simple implementation of the
   binary search algorithm:

auto binary_search(container const & v, value target)
{
  int left{0};
  int right{v.size()};
  int i{(left + right) / 2};
  while (left != right)
  {
    if (v[i] > target)
      right = i;
    else if (v[i] < target)
      left = i + 1;
    else
      return v[i];
    i = (left + right) / 2;
  }
  // not found
}
*/

/* Example of linear search:
   
auto linear_search(container const & v, value target)
{
  int i{0};
  while (i<v.size())
  {
    if (v[i] == target)
      return v[i];
    ++i;   
  }
  // not found
}
*/

int main()
{
    { // test linear search
        std::list<int> v { 1, 2, 5, 7, 11, 15 };
        assert (*sorted_search (v, 5) == 5);
        assert (*sorted_search (v, 1) == 1);
        assert (sorted_search (v, 6) == std::end(v));
        assert (*sorted_search (v, 15) == 15);
        assert (sorted_search (v, 100) == std::end (v));
    }

    { // test binary search
        std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        Binary<std::vector<int>> policy{};
        assert (*sorted_search(v, 6, policy) == 6);
        assert (*sorted_search(v, 3, policy) == 3);
        assert (*sorted_search(v, 9, policy) == 9);
        assert (*sorted_search(v, 1, policy) == 1);
        assert (*sorted_search(v, 11, policy) == 11);
        assert (sorted_search(v, 13,  policy) == std::end (v));
    }
}