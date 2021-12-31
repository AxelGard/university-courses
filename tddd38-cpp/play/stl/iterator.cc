#include <iostream>
#include <vector>

using namespace std;


void samething(vector<int> vec){
        
    // this is the same things as ** 
    for (auto it = begin(vec); it != end(vec); it++) {
        cout << *it << " ";
    }
    
    cout << endl;

    // ** this 
    for (auto num : vec) {
        cout << num << " ";
    }

    cout << endl;

}

void print_at(vector<int> vec, int i){
    auto&& val = next(begin(vec), i);
    cout << *val << endl; // needs to be de ref 
}

int main(){

    vector<int> vec{ 0,1,2,3,4 };

    samething(vec);
    
    print_at(vec, 2);

    return 0;
}