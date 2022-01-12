#include <cassert>
#include <set>
#include <string>
#include <vector>
#include <iostream>


template<typename LEFT, typename RIGHT>
auto concat_helper(LEFT& l, RIGHT& r, int, int) -> decltype(l += r, void()) {
    std::cout << "+= VERSION" << std::endl;
    l += r;
}

template<typename LEFT, typename RIGHT>
auto concat_helper(LEFT& l, RIGHT& r, float, int) -> decltype(l.push_back(*std::begin(r)), void()) {
    std::cout << "push_back VERSION" << std::endl;
    for (auto& v : r) {
        l.push_back(v);
    }
}

template<typename LEFT, typename RIGHT>
auto concat_helper(LEFT& l, RIGHT& r, float, float) -> decltype(l.insert(std::end(l),*std::begin(r)), void()) {
    std::cout << "INSERT VERSION" << std::endl;
    for (auto& v : r) {
        l.insert(std::end(l), v);
    }
}

template<typename LEFT, typename RIGHT>
void concat(LEFT& l, RIGHT& r) {
    concat_helper(l, r, int{}, int{});
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
        assert(s == "abcda");
    }

    {
        std::string str {"a"};
        std::set<char> s {'b', 'c', 'd'};

        concat(s, str); // use version 3
        concat(str, s); // use version 2
        assert(str == "aabcd");
    }
}
