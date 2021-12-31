/*
*
* Will need C++ 20 compiler, this will not compile but shows how to use concept vs SFINEA 
*
* source = https://youtu.be/dR64GQb4AGo
*/

#include <type_traits>
#include <iostream>

// SFINAE C++ <= 17 
template<typename T,
        typename = std::enable_if_t<std::is_floating_point_v<T>>> // this is not readable 
auto add(const T a, const T b){
    return a + b;
}


// C++ 20 requires clasuse 
template<typename T> 
auto add(const T a, const T b) requires std::is_floating_point_v<T> // will only be sellected if requires is true
{
    return a + b;
}


// C++ 20 concepts 
template<typename T>
concept floating_point = std::is_floating_point_v<T>;

template<floating_point T> // concept instead of typename 
auto add(const T a, const T b) 
{
    return a + b;
}


// implicit template func, using same concepts floting_point 
floating_point auto add(const floating_point auto a, const floating_point auto b) // this will however allow to take a double or float in combination (1.2f, 1.2) or (1.2,1.2f)
{
    return a + b;
}

// making concepts with namespace 
namespace std {
    template <typename T>
    concept integral = std::is_integral_v<T>;

    template<typename T>
    concept floating_point = std::is_floating_point_v<T>;

}
// will be used as 
template<std::integral T> // concept from namespace std instead of typename 
auto add(const T a, const T b) 
{
    return a + b;
}

// multi requeriements in concept 
template <typename T>
concept like_int = std::integral<T> || 
                   std::is_convertible_v<T, int>; // is a clause os || and && works 


// will unsrure that template has a at func aka t[i]
template <typename T> 
concept has_at = requires(T t, int i)
{
    t.at(i);// will not be run, only examen by the compiler 
}; // will return bool 

template<has_at T> // ensures by concepts that type has at using concept 
auto get(T t, int i){
    return t.at(i); // will be run 
}


template<typename T> 
concept has_iterator = requires(T t){
    {std::begin(t) } -> std::input_iterator; // ensures that return type is a iterator 
};

// concepts are boolian expresions 
template <has_iterator T>
auto&& get(T&& t, int i){
    if constexpr (has_at<T>) // will be checked att compile time and is fine to do due to concepts being booblian expression that can examen at compile time.
        return t.at(i);
    else 
        return *std::next(std::begin(t), i);
}

// same but using SFINEA
template<typename T>
auto&& get_helper(T&& t, int i, int) -> decltype(( t.at(i) ))
{
    return t.at(i);
}

template<typename T>
auto get_helper(T&& t, int i, long) -> decltype(( *std::next(std::begin(t), i) )) 
{
    return *std::next(std::begin(t), i);
}

template<typename T>
auto&& get(T&& t, int i)
{
    return get_helper(std::forward<T>(t), i, 0);
}

// concepts with classes 

template<typename From, typename To>
concept convertable = std::is_convertible<From, To>;

template<convertable<int> T> // int is the To, and the T is the From 
    requires std::copy_constructible<T>
    class Cls {

    };





int main (){

    add(1.1f, 2.2f);

    floating_point auto val =  add(1.1f, 2.2f);

    return 0;
}