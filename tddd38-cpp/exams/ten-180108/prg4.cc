#include <iostream>
#include <vector>
#include <iterator>


using namespace std;

template<typename It, typename Predicate>
class Enumerate_Iterator;

template<typename Container>
class enumerate {
    public: 
        using iterator = decltype(std::begin(std::declval<Container>()));

    private:    
        Container const con;

    public:
        enumerate(Container con )
         : con{con}
         { }

    Enumerate_Iterator<It, Predicate>& begin() {
        
    } 

    Enumerate_Iterator& end(){

    }
};

template<typename It, typename Predicate>
class Enumerate_Iterator {

    public:
        using value_type = iterator_traits<It>::value_type;
        using iterator_category = std::forward_iterator_tag<It>;
        using difference_type = std::ptrdiff_t;
        using pointer = It*;
        using reference = It&;

    private:
        It begin, end;
        Predicate pred;
        
        template<typename It, typename Predicate>
        friend class enumerate;

        Enumerate_Iterator(It b, It e, Predicate p)
            : begin{b}, end{e}, pred{p}
            { }

    public: 
        

        reference operator++(){
            this->begin++;
            return this->begin;
        }

        Enumerate_Iterator operator++(int){
            Enumerate_Iterator tmp{*this};
            ++(*this);
            return tmp;
        }

        pointer operator->(){
            return &(*this->begin);
        }

        


};

int main()
{
    std::vector vals {2,3,2,4,6,7};

    for ( auto && [idx,val]: enumerate(vals) )
        cout << idx << ": " << val++ << endl;

    for ( auto i: vals )
        cout << i << ' ';
    cout << endl;

    for (auto [index, value]: enumerate(vals) )
        cout << index << ": " << value << endl;

    Enumerate_Iterator it{begin(vals)};
    cout << '\n' << it->first << ' ' << it->second << '\n';
    ++it;
    cout << it->first;
}
