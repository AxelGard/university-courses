#include <cassert>
#include <vector>
#include <list>
#include <forward_list>

int main()
{
    { // test reduce, map, and filter
        std::list<int> v { 1, 2, 3, 4, 5, 6 };
        auto r {make_range(v)};

        assert(reduce(map(filter(r, [](int n)
                                 { return n % 2 == 0; }),
                          [](int n) { return 2 * n; }), 0.0,
                      [](int n, int m) { return n + m; }) == 18);
    }

    { // test size, filter and range-based for-loop
        std::vector<double> v { -2.6, 7.0, 1.3, 15.2 };
        auto r {make_range(v)};

        assert(r.size() == 4);
        filter(r, [](double x) { return x < 0.0; });
        assert(r.size() == 3);

        int i{0};
        for (double x : r)
        {
            assert(x == v[i++]);
        }
    }

    { // test take and skip
        std::forward_list<int> v { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto r {make_range(v)};

        int n{0};
        for (int x : r.take(3))
        {
            assert(x == n++);
        }

        for (int x : r.skip(3))
        {
            assert(x == n++);
        }
    }

}
