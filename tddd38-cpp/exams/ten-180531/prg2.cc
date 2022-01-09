#include <array>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <string>
#include <iostream>

using namespace std;



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