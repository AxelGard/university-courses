#include <array>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <string>
#include <iterator>
#include <iostream>

using namespace std;

template <typename LEFT_IT, typename RIGHT_IT, typename A, typename B>
struct zip_iterator {
    LEFT_IT left_iterator;
    RIGHT_IT right_iterator;

    zip_iterator(LEFT_IT left_iterator, RIGHT_IT right_iterator)
        : left_iterator{left_iterator}, right_iterator{right_iterator}
    { }

    bool operator==(const zip_iterator& other){
        return left_iterator == other.left_iterator && right_iterator == other.right_iterator;
    }

    bool operator!=(const zip_iterator& other){
        return !operator==(other);
    }

    std::pair<A&, B&> operator++(){
        left_iterator++;
        right_iterator++;
        return operator*();
    }

    std::pair<A&, B&> operator++(int){ // return before change
        std::pair<A&, B&> tmp = operator*();
        left_iterator++;
        right_iterator++;
        return tmp;
    }

    std::pair<A&, B&> operator*(){
        return std::pair<A&, B&>{*left_iterator, *right_iterator};
    }
};

template <typename LEFT, typename RIGHT>
struct zip {
    LEFT left_container;
    RIGHT right_container;
    using z_iterator = zip_iterator<typename LEFT::iterator, typename RIGHT::iterator,
        typename LEFT::value_type, typename RIGHT::value_type>;


    zip(LEFT left_container, RIGHT right_container)
        : left_container{left_container}, right_container{right_container}
    {}

    z_iterator begin(){
        return z_iterator{std::begin(left_container), std::begin(right_container)};
    }

    z_iterator end(){
        return z_iterator{std::end(left_container), std::end(right_container)};
    }
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
        /*
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
*/
}
