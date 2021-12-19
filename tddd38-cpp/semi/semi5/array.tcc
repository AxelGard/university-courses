
#include <cstddef> // size_t


template <typename T, size_t N>
size_t Array <T, N>::size(){
    return N;
}


template <typename T, size_t N>
T& Array <T, N>::operator[](size_t i) {
    return data[i];
}