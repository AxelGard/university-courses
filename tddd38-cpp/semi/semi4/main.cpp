#include <iostream>
using namespace std;

// func templates 
template <typename T, unsigned N>
T sum(T (&array)[N])
{
    T result{};
    for (int i{0}; i < N; ++i)
    {
        result += array[i];
    }
    return result;
}

template <> 
string sum(string (&array)[3]){
    string result{};
    for (int i{0}; i < 3; ++i)
    {
        result += array[i] + " ";
    }
    return result;
}


int main(){
    int arr1[4] { 5, 5, 5 ,5};
    double arr2[3] { 1.05, 1.05, 1.04 };
    string arr3[3] { "h", "i", "!" };
    cout << sum(arr1) << endl;
    cout << sum(arr2) << endl;
    cout << sum(arr3) << endl;
    return 0;
}