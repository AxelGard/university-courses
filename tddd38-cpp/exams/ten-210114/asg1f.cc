// four values
// int gcd(int a, int b, int c, int d)
// {
//     return gcd(gcd(a, b), c, d);
// }

// three values
// int gcd(int a, int b, int c)
// {
//     return gcd(gcd(a, b), c);
// }

// two values
// int gcd(int a, int b)
// {
//     if (b == 0)
//     {
//         return a;
//     }
//     return gcd(b, a % b);
// }

template <int a, int b, int... rest>
struct GCD
{
    static int const value = GCD<GCD<a, b>::value, rest...>::value;
};

template <int a, int b>
struct GCD<a, b>
{
    static int const value = GCD<b, a % b>::value;
};

template <int a>
struct GCD<a, 0>
{
    static int const value = a;
};

int main()
{
    static_assert(GCD<7, 3>::value == 1);
    static_assert(GCD<366, 60>::value == 6);
    static_assert(GCD<5187, 61446, 266>::value == 133);
    static_assert(GCD<6, 120, 36, 24, 366>::value == 6);
}