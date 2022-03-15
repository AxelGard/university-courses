# Program 3

## Repeated mistakes 
---
Did not use `template <typename... Ts>` instead of `template<typename T, typename Y, typename U>` this means that you can use
```cpp
int insert(Ts... ts){
    rows[id] = row_type{ts...};
}
``` 
---

Forgot to return referenced **&** row_type in get `row_type& get(int id)`. 

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
        if (f(id, std::forward<row_type>(row))){
            result.push_back(id)
        }
    }
    return result;
}
```

