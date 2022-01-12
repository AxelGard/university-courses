#include <algorithm>
#include <cassert>
#include <iterator>
#include <map>
#include <vector>
#include <list>

template <typename Container, typename Compare = std::less<typename Container::value_type>>
class sorted_insert_iterator
{
public:

    using value_type = Container::value_type;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    using iterator_category = std::output_iterator_tag;

    using iterator_ = sorted_insert_iterator<Container, Compare>;

        
    private:
    
        Container& container;
        Compare comp;
    
    public:

        sorted_insert_iterator(Container& c, Compare comp = {})
            : container{c}, comp{comp}
        { }
        
        iterator_& operator*()
        {
            return *this;
        }

        iterator_& operator++()
        {
            return *this;
        }

        iterator_& operator++(int)
        {
            return *this;
        }

        iterator_& operator=(typename value_type const& value)
        {
            auto it {std::lower_bound(std::begin(container), std::end(container), value, comp)};
            container.insert(it, value);
            return *this;
        }
  
};

template <typename Container, typename Compare = std::less<typename Container::value_type>>
sorted_insert_iterator<Container, Compare> sorted_inserter(Container& c, Compare comp = {})
{
    return sorted_insert_iterator<Container, Compare>{c, comp};
}

int main()
{
    std::map<int, int> m { {1, 2}, {5, 6} };
    *sorted_inserter(m)++ = std::make_pair(3, 4);
    assert((m == std::map<int, int>{{1,2},{3,4},{5,6}}));

    std::list<std::string> lst { "c" };
    *sorted_inserter(lst)++ = "a";
    *sorted_inserter(lst)++ = "b";
    *sorted_inserter(lst)++ = "a";
    assert((lst == std::list<std::string>{"a", "a", "b", "c"}));

    std::vector<int> v { 5, 7, 3, 12, -5 };

    {
        std::vector<int> res {};
        copy(std::begin(v), std::end(v), sorted_inserter(res));
    
        assert((res == std::vector<int>{-5, 3, 5, 7, 12 }));
    }

    {
        std::vector<int> res {};
        copy(std::begin(v), std::end(v), sorted_inserter(res, std::greater<int>{}));
        assert((res == std::vector<int>{12, 7, 5, 3, -5}));
    }

    {
        std::vector<int> res {};
        copy(std::begin(v), std::end(v),
             sorted_inserter<std::vector<int>, std::greater<int>>(res));
        assert((res == std::vector<int>{12, 7, 5, 3, -5}));
    }
}