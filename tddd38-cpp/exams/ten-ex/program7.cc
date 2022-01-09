#include <algorithm>
#include <iterator>
#include <vector>
#include <map>
#include <list>
#include <cassert>

/* Implement your classes here */
using namespace std;


template <typename It, typename Predicate>
class filter_iterator{
    using difference_type = void;
    using value_type = typename It::value_type;
    using reference = typename It::reference;
    using pointer = typename It::pointer;
    using iterator_category = std::input_iterator_tag;

    private:

        filter_iterator(It b, It e, Predicate p)
            : begin{b}, end{e}, pred{p}
            {
                begin = std::find_if(begin, end, pred);
            }

        template <typename Container, typename P>
        friend class filter;
        
        It begin;
        It end;
        Predicate pred;

    public: 

        bool operator==(filter_iterator const& other) const {   
            return begin == other.begin && end == other.end;
        }

        bool operator!=(filter_iterator const& other) const {
            return !(*this == other);
        }

        filter_iterator& operator++(){
            ++(this->begin);
            this->begin = find_if(this->begin, this->end, this->pred);
            return *this;
        }

        filter_iterator operator++(int){
            filter_iterator tmp{*this};
            ++(*this);
            return tmp;
        }

        value_type operator*(){
            return *this->begin;
        }

        pointer operator->(){
            return &(*this->begin);
        }

};

template <typename Container, typename Predicate>
class filter{
    using iterator = decltype(std::begin(std::declval<Container>()));

    private:
        Container const& c;
        Predicate p;

    public:
        filter(Container const& c, Predicate p)
        :  c{c}, p{p}
        { }

        filter_iterator<iterator, Predicate> begin(){
            return {std::begin(this->c), std::end(this->c), this->p};
        }

        filter_iterator<iterator, Predicate> end(){
            return {std::end(this->c), std::end(this->c), this->p};
        }
};

int main()
{
  {
    std::vector<int> arr = { -17, 1, -1, 5, 7, -2, 3 };
    std::vector<int> result{};
    for (auto n : filter(arr, [](auto n) { return n > 0; }))
    {
      result.push_back(n);
    }

    assert((result == std::vector<int>{1, 5, 7, 3}));
  }
  
  {
    std::map<std::string, int> m = { {"ABC", 123}, {"DEF", 45}, {"GHI", 6} };
    std::vector<std::pair<std::string, int>> result{};
    for (auto p : filter(m, [](auto p) { return p.second > 10; }))
    {
      result.push_back(p);
    }

    assert((result == std::vector<std::pair<std::string, int>>{{"ABC", 123}, {"DEF", 45}}));
  }

  {
    std::list<double> l = {3.5, 5.4, -1.0};
    std::vector<double> result{};
    for (auto x : filter(l, [](auto) { return false; }))
    {
      result.push_back(x);
    }

    assert(result.empty());
  }

  {
    std::vector<int> source { 1, 2, 3, 4, 5, 6 };
    std::vector<int> destination {0, 0, 0};
    filter f{source, [](auto n) { return n % 2 == 0; }};
    std::copy(f.begin(), f.end(), destination.begin());
  }

  {
    std::vector<std::string> v {"a", "bc", "def", "ghij"};
    filter f{v, [](auto&& str) { return str.size() == 3; }};
    assert(f.begin()->size() == 3);
  }
}