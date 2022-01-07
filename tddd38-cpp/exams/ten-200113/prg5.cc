#include <cassert>

int start(int x, int y)
{
    return x + y;
}

double step(int x)
{
    return 0.5 * x;
}

int last(double x)
{
    return x;
}

template <typename... Fs>
struct call_chain_result;


template<typename F, typename... Fs>
struct call_chain_result<F, Fs...>
{
    private:
        F f;
        call_chain_result<Fs...> rest;
    public:
        call_chain_result(F f, Fs... fs)
        : f{f}, rest{fs...}
        { }

        template<typename... Args>
        auto operator() (Args&&... args){
            return rest(f(std::forward<Args>(args) ... ))
        }
}

template <typename F>
struct call_chain_result{
private:
    F f;
public:
    call_chain_result(F f)
        : f{f}
        { }

    template<typename... Args>
    auto operator() (Args&&... args){
        return rest(f(std::forward<Args>(args) ... ))
    }

}

template<typename... Fs>
auto call_chain(Fs... fs){
    return call_chain_result<Fs...>{fs...};
}

int main()
{
    {
        int x{};
        auto f = call_chain(start, step, last);
        assert(f(x, x) == 0);

        x = 1;

        assert(f(x, x) == 1);

        assert(f(1, 4) == 2);
        assert(f(3, 3) == 3);
    }

    {
        auto f = call_chain([](int x = 1, int y = 1, int z = 1) { return x * (y + z); },
                            [](int x) { return x * x; },
                            [](int x) { return x - 1; });

        assert(f() == 3);
        assert(f(2) == 15);
        assert(f(2, 3) == 63);
        assert(f(2, 3, 4) == 195);
    }

    {
        auto f = call_chain(call_chain([]() { return 1; },
                                       [](int x) { return x + 1; }),
                            [](int x) { return x * x; });

        assert(f() == 4);
    }
}
