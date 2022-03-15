# Repeated mistakes

*from 2022-01-13*

## From Asg 1 

Did not know how to read in file using `ifstream` 
```cpp
std::istream& operator>>(std::istream &is, Event &event)
    {
        is >> std::get<0>(event) >> std::get<1>(event);
        std::getline(is, std::get<2>(event));
        return is;
    }
```
and `istream_iterator`
```cpp
ifstream first_ifs{"first.txt"};
vector<Event> first_schedule{
    istream_iterator<Event>{first_ifs},   // gets everything from file 
    istream_iterator<Event>{}             // using input stream iterator 
};

```

---

Print iterator by copy, using `std::copy` and `std::ostream_iterator<Event>{std::cout, "\n"}`

```cpp
vector<int> ex = {1,2,3}
copy(  ex.begin(), ex.end(),
       ostream_iterator<int>{std::cout, "\n"}
       );                //  write to   end with  
```

---
Did not understand how to set up `istream` and `ostream` operator. 

```cpp
namespace std
{
    std::istream& operator>>(std::istream &is, Event &event)
    {
        ...
        return is;
    }

    std::ostream& operator<<(std::ostream &os, Event const &event)
    {
        ...
        return os;
    }
}
```

## From Asg 3

---
Did not use `template <typename... Ts>` instead of `template<typename T, typename Y, typename U>` this means that you can use
```cpp
int insert(Ts... ts){
    rows[id] = row_type{ts...};
}
``` 
---

Forgot to return referenced **&** row_type in get `row_type& get(int id)`. 

```cpp
struct cls{
    vector<int> rows;
    int& get(int idx) { // Don't forget int& !!! 
        return rows[idx]
    }
};
```

--- 

Did not updated remove to use iterator instead of actual range. 

```cpp
auto it{rows.find(id)};
rows.erase(it);
```
---

Did not know how to create the *filter* function, did not know how to take a lambda as a argument. 

```cpp
template<typename F> 
vector<int> filter(F &&f)
{
    vector<int> result{};
    for (auto &&[id, row] : rows) // obs C++ 17 
    {
        if (f(id, std::forward<row_type>(row))){ // must use forward 
            result.push_back(id);
        }
    }
    return result;
}
```
---

## From Asg 4

Function taking string should use `const&` 

```cpp
class cls{
    string const name;
    cls(string const& name) // Name should not change later
        : name{name}       // so need to be const. 
        { } 
};
```

---

Grid need to take int as parameter in template by `template <int width> class Grid : public Collection {};`
```cpp
template <int width>
class Grid : public Collection
{
    ...
};

Grid<3> grid{"My Grid (3)"};
```

---

List can then be done with `class List : public Grid<1> {};`

```cpp
class List : public Grid<1>{ // inherent from Grid initialized with 1.  
    ... 
};
```

---

Parent constructor can be used by 

```cpp
class Parent { ... };

class Child : public Parent {
    using Parent::Parent; // uses parent constructor 
};
```





