#include <array>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <string>
#include <iostream>
#include <utility>
#include <functional>
#include <iterator>

using namespace std;

template <typename LeftIt, typename RightIt>
class zip_iterator
{

    using left_traits  = iterator_traits<LeftIt>;
    using right_traits = iterator_traits<RightIt>;

public:

    using difference_type = void;
    using value_type = pair<
        typename left_traits::value_type  &,
        typename right_traits::value_type &>;
    using pointer = void;
    using reference = value_type;
    using iterator_category = forward_iterator_tag;

    zip_iterator() = default;

    zip_iterator<LeftIt, RightIt> & operator++()
    {
        ++lbegin;
        ++rbegin;
        if (lbegin == lend || rbegin == rend)
        {
            lbegin = lend;
            rbegin = rend;
        }
        return *this;
    }

    zip_iterator<LeftIt, RightIt> & operator++(int)
    {
        auto tmp {*this};
        ++*this;
        return tmp;
    }

    reference operator*()
    {
        return value_type{*lbegin, *rbegin};
    }

    bool operator==(zip_iterator const & other) const
    {
        return (lbegin == other.lbegin &&
                rbegin == other.rbegin);
    }
    bool operator!=(zip_iterator const & other) const
    {
        return !(*this == other);
    }

private:

    zip_iterator(LeftIt lbegin , LeftIt lend,
                 RightIt rbegin, RightIt rend)
        : lbegin {lbegin}, lend {lend},
          rbegin {rbegin}, rend {rend}
    {
    }

    LeftIt  lbegin{};
    LeftIt  lend  {};
    RightIt rbegin{};
    RightIt rend  {};

    template <typename A, typename B>
    friend class zip;

};

template <typename Left, typename Right>
class zip
{

    using LeftIt  = typename Left::iterator;
    using RightIt = typename Right::iterator;

public:

    zip(Left & left, Right & right)
        : first {std::begin(left) , std::end(left),
                 std::begin(right), std::end(right)},
        last {std::end(left) , std::end(left),
              std::end(right), std::end(right)}
    {
    }

    zip_iterator<LeftIt, RightIt> begin() { return first; }
    zip_iterator<LeftIt, RightIt> end() { return last; }

private:

    zip_iterator<LeftIt, RightIt> first, last;

};

int main()
{
    vector<int> v { 1, 2, 3, 4 };
    array<string, 4> a { "this", "is", "a", "test" };
    list<int> l { 8, 9, 10, 11, 12, 13 };
    map<string, int> m { {"try", 5}, {"to", 6}, {"zip", 7} };

    cout << "vector zip array: \n";
    for (auto [left, right] : zip(v, a))
        cout << '(' << left << ", " << right << ")\n";

    cout << "vector zip list: \n";
    for (auto [left, right] : zip(v, l))
        cout << '(' << left << ", " << right << ")\n";

    cout << "vector zip map: \n";
    for (auto [left, right] : zip(v, m))
        cout << '(' << left << ", ("
             << right.first << ", " << right.second
             << "))\n";

    cout << "map zip map: \n";
    for (auto [left, right] : zip(m, m))
        cout << "(("
             << left.first << ", " << left.second
             << "), ("
             << right.first << ", " << right.second
             << "))\n";

    // check if modification is possible

    zip zipped {v, m};
    for (auto it {zipped.begin()}; it != zipped.end(); ++it)
    {
        (*it).first = 0;
    }

    // should print: 0 0 0 4
    for (auto i : v)
    {
        cout << i << ' ';
    }
    cout << endl;
}
