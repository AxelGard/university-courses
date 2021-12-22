# Theory questions 

## fold-expressions 

a fold-expression is denoted by `...` and is used for passing 
mulitple arguments to a function in a template. 
example 

```cpp
template<typename ...Args>
sum(Args... args) {
	return(args + ...);
}
```

## override keyword

the override keyword overrides a virtual function from a class. 
So for example if you inherent a class but one of the functions that was defined in the parent class 
we don't want to be implement in the same way so we override it using the override keyword. 


## calling a refrence method 

```cpp
struct A
{
	void foo() &&;
};

A{}.foo();

```

## type vs non-type template parameter 

```cpp
//nontype
template<typename T>
auto add(T a, T b){
	return a + b;
}

//type
template<typename T, int i>

```

non type is chosen a compile time and type is static. 


## Slicing 

when base class is removed from derived class, 
those removing attribute that was in the base class. 





