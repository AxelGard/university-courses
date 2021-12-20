#include <cassert>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename T, typename U>
auto join_helper(T const& lhs, U const& rhs, int)
    -> vector<decltype(make_pair(*begin(lhs), *end(rhs)))>
{
    using join_pair = pair<typename T::value_type,
                           typename U::value_type>;

    vector<join_pair> result {};
    
    auto lhs_it { begin(lhs) };
    auto rhs_it { begin(rhs) };

    while (lhs_it != end(lhs) && rhs_it != end(rhs))
    {
        result.emplace_back(*lhs_it, *rhs_it);
        ++lhs_it;
        ++rhs_it;
    }

    return result;
}

template <typename T, typename U>
auto join_helper(T const& lhs, U const& rhs, float)
    -> vector<decltype(make_pair(*begin(lhs), rhs))>
{
    using join_pair = pair<typename T::value_type, U>;
    vector<join_pair> result {};
    for (auto&& e : lhs)
    {
        result.emplace_back(e, rhs);
    }
    return result;
}

template <typename T, typename U>
auto join_helper(T const& lhs, U const& rhs, float)
    -> vector<decltype(make_pair(lhs, *begin(rhs)))>
{
    using join_pair = pair<T, typename U::value_type>;
    vector<join_pair> result {};
    for (auto&& e : rhs)
    {
        result.emplace_back(lhs, e);
    }
    return result;
}

template <typename T, typename U>
auto join(T const& lhs, U const& rhs)
{
    return join_helper(lhs, rhs, 0);
}

int main()
{
    {
        vector<int> v { 1, -2, 3 };
        string s { "hello world" };
        
        auto res = join(v, s);
        assert( (res == vector<pair<int, char>>{ {1, 'h'}, {-2, 'e'}, {3, 'l'} }) );
    }

    {
        list<string> l { "hello", "there" };
        int x { 3 };

        auto left = join(l, x);
        assert( (left == vector<pair<string, int>>{ {"hello", 3}, {"there", 3} }) );

        auto right = join(x, l);
        assert( (right == vector<pair<int, string>>{ {3, "hello"}, {3, "there"} }) );
    }
}

// Explain what C++20 Concepts are and how they could have been used
// to simplify the implementation of join.

// C++20 concepts are a new type of entity in the language that
// encapsulates requirements on data types. This is used to apply
// these requirements on template type paramaters. This was previously
// done with a combination of SFINAE, function overload resolution and
// template parameter specializations.

// This problem would have been a lot easier to implement as well as
// easier to understand with the help of C++20 concepts. Primarily
// because we can use C++20 concepts to apply requirements to detect
// containers. For example:

// template <typename T>
// concept Container = requires (T t) {
//     std::begin(t);
//     std::end(t);
//     typename T::value_type value;
// };

// template <Container LHS, Container RHS>
// auto join(LHS const& lhs, RHS const& rhs);

// template <Container LHS, typename RHS>
// auto join(LHS const& lhs, RHS const& rhs);

// template <typename LHS, Container RHS>
// auto join(LHS const& lhs, RHS const& rhs);

// This way we don't have to use expression SFINAE, nor do we have to
// introduce a priority.