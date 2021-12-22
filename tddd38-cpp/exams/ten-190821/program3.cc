#include <vector>
#include <list>
#include <set>
#include <cassert>
#include <iostream>
#include <string>



int main()
{
  {
    std::vector<int> v1 {1,2,3};
    std::vector<int> v2 {4,5,6};
    std::vector<int> result{};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{1,2,3,4,5,6}));
  }
  {
    std::list<int> v1 {};
    std::list<int> v2 {4,5,6};
    std::vector<int> result{};
    append a{v1, v2};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{4,5,6}));
  }
  {
    std::set<int> v1 {1,2,3};
    std::set<int> v2 {};
    std::vector<int> result{};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{1,2,3}));
  }
  {
    std::vector<int> v1 {};
    std::vector<int> v2 {};
    std::vector<int> result{};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{}));
  }
  {
    std::vector<std::string> v1{"a"};
    std::vector<std::string> v2{"b"};
    append<std::vector<std::string>> a{v1, v2};
    auto it{a.begin()};
    assert(*it == "a");
    assert(*it++ == "a");
    assert(*it == "b");
  }
}