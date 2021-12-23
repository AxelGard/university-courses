#include <cassert>
#include <set>
#include <string>
#include <vector>

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