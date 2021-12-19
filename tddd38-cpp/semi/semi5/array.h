#include <cstddef> // size_t
template <typename T, size_t N>
class Array {
    public:
        static size_t size();
        T& operator[](size_t i);
    private:
        T data[N]{};
};

#include "array.tcc"