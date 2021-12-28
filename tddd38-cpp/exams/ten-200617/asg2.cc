#include <cassert>
#include <set>
#include <string>
#include <vector>
#include <iostream>



namespace details
{
    template <typename Left, typename Right>
    auto concat(Left& left, Right const& right, int, int)
        -> decltype(left += right, std::declval<void>())
    {
        left += right;
    }

    template <typename Left, typename Right> 
    auto concat(Left& left, Right const& right, int, float)
     -> decltype(left.push_back(*std::begin(right)), std::declval<void>()){
        for (auto&& element : right) {
            left.push_back(element);
        }
    }

    template <typename Left, typename Right> 
    auto concat(Left& left, Right const& right, float, float)
     -> decltype(left.insert(std::end(left), *std::begin(right)), std::declval<void>()){
        for (auto&& element : right) {
            left.insert(std::end(left), element);
        }
    }

} 




template<typename Left, typename Right> 
void concat(Left& left, Right const& right){
    details::concat(left, right, 0, 0);
}



int main()
{

    {
        std::string s1 {"a"};
        std::string s2 {"b"};

        concat(s2, s1); // use version 1
        concat(s1, s2); // use version 1
        assert(s1 == "aba");
    }

    {
        std::string s {"a"};
        std::vector<char> v {'b', 'c', 'd'};

        concat(v, s); // use version 2
        concat(s, v); // use version 2
        //assert(s == "abcda");
    }

    {
        std::string str {"a"};
        std::set<char> s {'b', 'c', 'd'};

        concat(s, str); // use version 3
        concat(str, s); // use version 2
        //assert(str == "aabcd");
    }
}