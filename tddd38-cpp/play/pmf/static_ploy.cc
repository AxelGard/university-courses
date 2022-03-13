#include <iostream>


using namespace std;




template<typename T>
struct A {
    
    A(int a):a{a} { }

    int a;

    void print(ostream& os) {
        static_cast<T*>(this)->print(os);
    }
};

struct B : public A<B> {
    using A::A;
    void print(ostream& os) {
       os << " static ploy more phism ??";
    }
};

struct C : public A<C> {
    using A::A;
    void print(ostream& os) {
       os << " is this ";
    }
};



int main(){

    C c{3};
    c.print(cout);
    cout << endl;

    B b{2};
    b.print(cout);
    cout << endl;

 
    return 0;
}