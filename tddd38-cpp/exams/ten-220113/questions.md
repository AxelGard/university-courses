# Questions 

## Discussion 2 

### a) When overloading operator<< what are some things you must consider?

The function takes in a reference to output stream and this same stream should be returned to from the function. 
Ex : 
```cpp
ostream& operator<<(ostream& os) {
    os << this->someValue; 
    return os;
}
```
Because the function takes a reference of a stream and the same stream is return this allows chaining of the function. 
so we can do 
```cpp
std::cout << "hello" << " " << "world" << std::endl;
```  


### b) Explain what overload resolution is.

When the compiler finds that is has multiple choices for a function call then it will choose a function based on what are the input parameters and can they be converted with out loss of data ex `(int) 1 --> (float) 1.0` there is no loss of data. 

The compiler will also check for the number of arguments and try to match on dose, as well as the return type. 

The compiler is trying to get a perfect matching of the input to the function and the output. 

The compiler checks through 
 1. function name. (the name must be a perfect matching off course)
 2. input types for the args. 
 3. nr of args. 
 4. return type.

For templates all the variables dose not need to be a explicit type, 
so that variables type will be set by the compiler. 

---

## Discussion 5 

### Discuss the advantages and disadvantages of the class hierarch

The code from `prg4.cc` is quite expandable due to the use of 
dynamic polymorphism. For example expanding the program with example 
of class row would not be difficult at all. 

```cpp
class Row : public Collection {
    
};
```

Then we could add needed methods and override some functions like 
print_item` and there you go, the code was extended with a new feature. 

This code will also become quite readable however due to the use of dynamic
polymorphism we will lose quite a lot of performance compared to static 
polymorphism. Were we use templates as a way of getting polymorphism. 

Static polymorphism means that you will keep the extendability 
but dose not lose performance.
dynamic polymorphism comes at the cost of taking more memory due 
to a larger v-table and dynamic binding in run time. 






