/*
*
* Will need C++ 20 compiler 
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

template<floating_point T> 
auto add(const T a, const T b) 
{
    return a + b;
}


// implicit template func, using same concepts floting_point 
floating_point auto add(const floating_point auto a, const floating_point auto b) // this will however allow to take a double or float in combination (1.2f, 1.2) or (1.2,1.2f)
{
    return a + b;
}


int main (){

    add(1.1f, 2.2f);

    floating_point auto val =  add(1.1f, 2.2f);

    return 0;
}