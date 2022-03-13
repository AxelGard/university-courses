#include <iostream>
#include <vector>

using namespace std;

struct A {
    static int cnt;

    void print(ostream& os){
        cnt++;
        os << cnt << endl;;
    }

    static void print_(ostream& os){
        cnt++;
        os << cnt << endl;;
    }
};

int A::cnt = 0;

int main() {

    vector<A*> vecA {
        new A{},
        new A{},
        new A{}
    };

    for (auto v : vecA)
        v->print(cout);
    cout << endl;

    for (auto v : vecA)
        v->print_(cout);
    cout << endl;

    for (auto v : vecA)
        delete v;

    return 0;
}