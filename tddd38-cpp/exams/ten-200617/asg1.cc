
template<typename T, int N>
struct Pair
{

};


template<typename ... T>
struct List {

};

template<typename, typename>
struct Lookup;

template<typename T, typename U, int N, typename... Ts>
struct Lookup<T, List<Pair<U, N>, Ts...>>{
    static int const value = Lookup<T, List<Ts...>>::value;
};

template<typename T, int N, typename... Ts>
struct Lookup<T, List<Pair<T, N>, Ts...>>{
    static int const value = N;
};

int main()
{
    using My_List = List<Pair<int, 3>, Pair<double, -4>, Pair<float, 12>>;

    static_assert(Lookup<int, My_List>::value == 3);
    static_assert(Lookup<double, My_List>::value == -4);
    static_assert(Lookup<float, My_List>::value == 12);
}