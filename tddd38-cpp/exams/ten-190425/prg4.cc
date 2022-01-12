#include <vector>
#include <string>
#include <set>
#include <list>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <cstddef>
#include <utility>
#include <iterator>


using namespace std;

template <typename T>
auto get_size(T&& t);

template<typename T>
auto get_size_helper(T&& t, int) ->decltype(std::begin(t), t.size()) {
    int result = 0;
    for(auto&& val : t)
        result += get_size(val);
    return result;
}

template<typename T>
auto get_size_helper(T&& t, double) ->decltype(sizeof(t),std::size_t()) {
    return sizeof(t);
}

template<typename T>
auto get_size(T&& t){
    return get_size_helper(std::forward<T>(t), int{});
}

template<typename... Ts>
std::size_t total_size(Ts&&... ts){
    return (get_size(std::forward<Ts>(ts)) + ... + 0);
}

int main()
{
    int x{};
    assert(get_size(x) == 4);

    assert(total_size(x) == 4);
    
    int arr[3] {1, 2, 3};
    assert(get_size(arr) == 12);
    assert(total_size(x, arr) == 16);
    
    string str {"this is a test"};
    assert(get_size(str) == 14);

    assert(total_size(x, arr, str) == 30);
    
    vector<string> v {"hello", "there are some characters", "in here"};
    assert(get_size(v.front()) == 5);

    assert(total_size(x, arr, str, v.front()) == 35);
    
    assert(get_size(v) == 37);

    assert(total_size(x, arr, str, v.front(), v) == 72);
    
    set<list<int>> s { {1, 2, 3}, {4, 5}, {6} };
    assert(get_size(s) == 24);

    assert(total_size(x, arr, str, v.front(), v, s) == 96);
}