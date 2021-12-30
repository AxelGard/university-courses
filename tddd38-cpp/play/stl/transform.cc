#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main(){

    vector<int> vec {1,2,3,4,5,6};

    //          start       stop     write to   func ptr or lambda 
    transform(begin(vec), end(vec), begin(vec), [](int value){
        return ++value;
    });    

    for (auto&& val : vec){
        cout << val << endl;
    }

    return 0;
}