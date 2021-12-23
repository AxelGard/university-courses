


int main()
{
    using My_List = List<Pair<int, 3>, Pair<double, -4>, Pair<float, 12>>;

    static_assert(Lookup<int, My_List>::value == 3);
    static_assert(Lookup<double, My_List>::value == -4);
    static_assert(Lookup<float, My_List>::value == 12);
}