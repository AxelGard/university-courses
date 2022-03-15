#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;
// TODO: Write description
namespace details
{

    template <typename Container, typename Callable>
    auto enumerate(Container const &container, Callable &&callable, int, int)
        -> decltype(callable(*std::begin(container), 0, container.size()), std::declval<void>())
    {
        cout << "container.size()" << endl;
        unsigned index{0};
        for (auto &&element : container)
        {
            callable(element, index, container.size());
            ++index;
        }
    }

    template <typename Container, typename Callable>
    auto enumerate(Container const &container, Callable &&callable, int, float)
        -> decltype(callable(*std::begin(container), 0), std::declval<void>())
    {
        cout << "*std::begin(container), 0)" << endl;
        unsigned index{0};
        for (auto &&element : container)
        {
            callable(element, index);
            ++index;
        }
    }

    template <typename Container, typename Callable>
    auto enumerate(Container const &container, Callable &&callable, float, float)
        -> decltype(callable(*std::begin(container)), std::declval<void>())
    {
        cout << "*std::begin(container)" << endl;
        for (auto &&element : container)
        {
            callable(element);
        }
    }
}

template <typename Container, typename Callable>
void enumerate(Container const &container, Callable &&callable)
{
    details::enumerate(container, std::forward<Callable>(callable), 0, 0);
}

/* Expected output:

1
2
3

0: 1
1: 2
2: 3

[ 0: 1
  1: 2
  2: 3 ]

[ abc: 6
  def: 5
  ghi: 4 ]

0: mno
1: jkl


*/

int function(int n)
{
    std::cout << n << '\n';
}

struct Functor
{
    void operator()(int n, unsigned index)
    {
        std::cout << index << ": " << n << std::endl;
    }
};

int main()
{

    // TESTCASE: vector<int>
    {
        std::vector<int> v{1, 2, 3};

        // function with element
        enumerate(v, function);

        std::cout << std::endl;

        // function object with element and index as parameters
        enumerate(v, Functor{});

        std::cout << std::endl;

        // lambda with element, index and size
        enumerate(v, [](int n, unsigned index, unsigned size)
                  {
                      // Print brackets around the list
                      if (index == 0)
                      {
                          std::cout << "[ ";
                      }
                      else
                      {
                          std::cout << "  ";
                      }
                      std::cout << index << ": " << n;
                      if (index + 1 == size)
                      {
                          std::cout << " ]\n";
                      }
                      else
                      {
                          std::cout << std::endl;
                      } });
    }
    std::cout << std::endl;

    // TESTCASE: map<string, int>
    {
        std::map<std::string, int> m{
            {"abc", 6},
            {"def", 5},
            {"ghi", 4}};

        // Test priority when there are ambigous function calls
        //
        // This is ambigous since all three versions of enumerate
        // would work. The version taking 3 parameters in the lambda
        // always takes first priority, so that is the version that
        // should be called.
        enumerate(m, [](std::pair<std::string, int> const &element,
                        unsigned index = 0, unsigned size = 0)
                  {
                      if (index == 0)
                      {
                          std::cout << "[ ";
                      }
                      else
                      {
                          std::cout << "  ";
                      }
                      
                      // Print key and value on each line
                      std::cout << element.first << ": " << element.second;
                      if (index + 1 == size)
                      {
                          std::cout << " ]\n";
                      }
                      else
                      {
                          std::cout << std::endl;
                      } });
    }
    std::cout << std::endl;

    // TESTCASE: C-array
    {
        // enumerate should work for C-arrays as well.
        std::string a[2]{"mno", "jkl"};

        // Test C-array and priority
        //
        // This lambda matches the versions of enumerate that takes 1
        // and 2 parameters in the lambda. The version with 2
        // parameters should take priority.
        enumerate(a, [](std::string const &str, unsigned index = 0)
                  { std::cout << index << ": " << str << std::endl; });
    }
}