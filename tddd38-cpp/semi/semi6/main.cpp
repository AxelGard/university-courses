#include<iostream>


struct Type { };

template <typename T> 
void foo(){
    typename T::type x{};
    Type T{};
}

template <int N> struct S1 
{
    template <int M> struct S2
    {
      static int foo() {
          return M;
      }
    };
    
};

int main(){

    S1<1>::S2<3>::foo();
    return 0;
}
