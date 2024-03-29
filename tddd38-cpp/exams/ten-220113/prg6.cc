#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

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
namespace details {
    template <typename C, typename F>
    auto enumerate(C c, F &&f, int, int) // used by C-arr and map<>
        -> decltype(f(*begin(c), 0, c.size()), declval<void>())
    {
        unsigned index{0};
        for (auto &&e : c){
            f(e, index, c.size());
            ++index;
        }
    }

    template <typename C, typename F>
    auto enumerate(C c, F &&f, int, float) 
        -> decltype(f(*begin(c), 0), declval<void>())
    {
        unsigned index{0};
        for (auto &&e : c)
        {
            f(e, index);
            ++index;
        }
    }

    template <typename C, typename F>
    auto enumerate(C c, F &&f, float, float)  // used by Vec  
        -> decltype(f(*begin(c)), declval<void>())
    {
        unsigned index{0};
        for (auto &&e : c)
        {
            f(e, index);
        }
    }
}

template <typename C, typename F>
void enumerate(C c, F &&f)
{
    details::enumerate(c, forward<C>(c), 0,0)
}

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