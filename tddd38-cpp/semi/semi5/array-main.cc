#include "array.h"

int main(){
    Array<int, 3> arr;
    for (size_t i{0}; i < arr.size(); ++i){
        arr[i] = i;
    }
}