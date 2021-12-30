#include <iostream>
#include <vector>


using namespace std;

void applay(const vector<int>& vec, void(*func)(int)){
    for (int val : vec){
        func(val);
    }
}

int main (){
    
    vector<int> vec {1,2,3,4};

    auto lambda = [](int val){
         cout << val << endl;
    };

    applay(vec, lambda);

    

    return 0;
}