#include <iostream>
#include <cassert>
#include <vector>
#include <iterator>
#include <string>
#include <list>
#include <set>

using namespace std;

template <typename It>
class append_iterator;

template<typename Container>
class append{

  private:
  
    Container& v1;
    Container& v2;

  public:

    append(Container& v1, Container& v2) : v1{v1}, v2{v2} { }

    using iterator = decltype(std::begin(Container{}));

    append_iterator<iterator> begin(){
      return { begin(v1), end(v1), begin(v2), end(v2) }
    }

    append_iterator<iterator> end(){
      return { end(v2), end(v1), begin(v2), end(v2) }
    }
};

template <typename It>
class append_iterator {

  private:
    template <typename T>
    friend class append;
    
    append_iterator(It cur, It end1, It beg2, It end2)
      : current{cur}, end1{end1}, beg2{beg2}, end2{end2}
      {
        if (this->current == end1)
          this->current = beg2;
      }

      It current;
      It end1;
      It beg2;
      It end2;

  public:
    using value_type = typename iterator_traits<It>::value_type;
    using difference_type = typename iterator_traits<It>::difference_type;
    using refernce = typename iterator_traits<It>::reference;
    using poiter = typename iterator_traits<It>::pointer;
    using iterator_category = input_iterator_tag; 

  public: 

    refernce operator*(){
      return *this->current;
    }

    append_iterator<It>& operator++(){
      ++this->current;
      if(this->current == end1)
        this->current = this->beg2;
      return *this;
    }

    append_iterator<It>& operator++(int){
      append_iterator<It> tmp{*this};
      ++(*this);
      return tmp;
    }

    bool operator==(append_iterator<It> const& other) const {
       return current == other.current &&  
              end1 == other.end1 && 
              beg2 == other.beg2 && 
              end2 == other.end2; 
    }

    bool operator!=(append_iterator<It> const& other) const {
      return !(*this == other);
    }



};


int main()
{
  {
    std::vector<int> v1 {1,2,3};
    std::vector<int> v2 {4,5,6};
    std::vector<int> result{};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{1,2,3,4,5,6}));
  }
  {
    std::list<int> v1 {};
    std::list<int> v2 {4,5,6};
    std::vector<int> result{};
    append a{v1, v2};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{4,5,6}));
  }
  {
    std::set<int> v1 {1,2,3};
    std::set<int> v2 {};
    std::vector<int> result{};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{1,2,3}));
  }
  {
    std::vector<int> v1 {};
    std::vector<int> v2 {};
    std::vector<int> result{};
    for (auto i : append(v1, v2))
    {
      result.push_back(i);
    }
    assert((result == std::vector<int>{}));
  }
  {
    std::vector<std::string> v1{"a"};
    std::vector<std::string> v2{"b"};
    append<std::vector<std::string>> a{v1, v2};
    auto it{a.begin()};
    assert(*it == "a");
    assert(*it++ == "a");
    assert(*it == "b");
  }
}