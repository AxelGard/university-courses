#include <iostream>
#include <cstddef> // size_t
#include <string>



// Class templates
template <typename T, size_t N>
class Array{
    public: 
        static size_t size(){
            return N;
        }

        T& operator [](size_t i){
            return data[i];
        }

        //Variadic Templates
        template <typename T, size_t N>
        template <typename... Ts>
        Array(Ts... list)
        : data{list...}
        { }


    private:
        T data[N]{};
};


int main(){
    Array<int,2> ar{};
    ar[0] = 1;
    std::cout << ar[0] <<std::endl; 

    return 0;
}