#include <vector>
#include <list>
#include <tuple>
#include <iterator>
#include <cassert>

namespace details
{
    template <int N, typename T>
    auto get(T&& t, int, int) -> decltype(std::get<N>(std::forward<T>(t)))
    {
        return std::get<N>(std::forward<T>(t));
    }

    template <int N, typename T>
    auto get(T&& t, int, float) -> decltype(t[N])
    {
        return t[N];
    }

    template <int N, typename T>
    auto get(T&& t, float, float) -> decltype(*std::next(std::begin(t), N))
    {
        return *std::next(std::begin(t), N);
    }
}

// this was put in a namespace to differentiate from std::get. Due to
// ADL (argument dependent lookup), when writing: get<0>(t) where t is
// an std::tuple the compiler will match std::get since it has a
// better overload (one taking std::tuple directly).
namespace my_get
{
    template <int N, typename T>
    auto get(T&& t)
    {
        return details::get<N>(std::forward<T>(t), 0, 0);
    }
}

int main()
{
    std::tuple<int, int, int> t { 0, 1, 2 };
    assert(my_get::get<0>(t) == 0);
    assert(my_get::get<1>(t) == 1);
    assert(my_get::get<2>(t) == 2);

    std::vector<int> v { 3, 2, 1, 0 };
    assert(my_get::get<0>(v) == 3);
    assert(my_get::get<1>(v) == 2);
    assert(my_get::get<2>(v) == 1);
    assert(my_get::get<3>(v) == 0);

    std::list<double> l { 1.2, 3.4, 5.6 };
    assert(my_get::get<0>(l) == 1.2);
    assert(my_get::get<1>(l) == 3.4);
    assert(my_get::get<2>(l) == 5.6);

    std::array<std::string, 3> a { "123", "45", "6789" };
    assert(my_get::get<0>(a) == "123");
    assert(my_get::get<1>(a) == "45");
    assert(my_get::get<2>(a) == "6789");

    char c[2] { 'a', 'b' };
    assert(my_get::get<0>(c) == 'a');
    assert(my_get::get<1>(c) == 'b');
}