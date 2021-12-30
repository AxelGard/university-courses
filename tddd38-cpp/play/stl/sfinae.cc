// sfinea

#include <iostream>
#include <vector>
#include <assert.h>


using namespace std;

// non template 
void foo(double n){
    cout << "from double" << endl;
}


// requires iterators 
template<typename Iter>
typename Iter::value_type sum(Iter b, Iter e, typename Iter::value_type acc){
    cout << "Iter b, Iter e, typename Iter::value_type acc " << endl;
    while (b != e)
    {
        acc += *b++; // add cur b then increase 
    }
    return acc;
}


// can take ptr
template<typename T>
T sum(T* b, T* e, T acc){
    cout << "T* b, T* e, T acc  " << endl;
    while (b != e)
    {
        acc += *b++; // add cur b then increase 
    }
    return acc;
}



int main(){

    foo(1.0);
    foo(1); // int still works fine, due to convertion 

    vector<int> a {1,2,3};
    
    auto res_it = sum(begin(a), end(a), 0); // normal call using iter 
    assert(res_it==6);

    auto res_ptr = sum(a.data(), a.data()+3, 0); // dose not have  Iter::value_type due to being a ptr 
     assert(res_ptr==6);
    return 0;
}