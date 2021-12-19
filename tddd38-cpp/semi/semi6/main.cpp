#include<iostream>


struct Type { };

template <typename T> 
void foo(){
    typename t::type x{};
    Type t{};
}

int main(){
    return 0;
}
