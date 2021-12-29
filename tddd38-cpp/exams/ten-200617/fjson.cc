#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <memory>

class JSON_Value;

template <typename T, typename... Args>
JSON_Value* create(Args&&... args);

class JSON_Value
{
public:
    virtual ~JSON_Value() = default;
    virtual void print(std::ostream& os) const = 0;
    virtual JSON_Value* copy() const = 0;
};

class JSON_Null : public JSON_Value
{
public:

    JSON_Null() = default;
    
    void print(std::ostream& os) const override
    {
        os << "null";
    }

    JSON_Value* copy() const override
    {
        return create<JSON_Null>();
    }
};

template <typename T>
class JSON_Data : public JSON_Value
{
public:

    JSON_Data(T const& value)
        : value { value }
    { }
    
    void print(std::ostream& os) const override
    {
        os << value;
    }

    JSON_Value* copy() const override
    {
        return create<JSON_Data<T>>(value);
    }
    
private:
    T value;
};

template <>
class JSON_Data<std::string> : public JSON_Value
{
public:

    JSON_Data(std::string const& value)
        : value { value }
    { }

    void print(std::ostream& os) const override
    {
        os << '\"' << value << '\"';
    }    
    
    JSON_Value* copy() const override
    {
        return create<JSON_Data<std::string>>(value);
    }
    
private:
    std::string value;
};

class JSON_Object : public JSON_Value
{
    
public:

    ~JSON_Object() override
    {
        for (auto&& [key, value] : fields)
        {
            delete value;
        }
    }
    
    void print(std::ostream& os) const override
    {
        os << '{';

        bool first { true };
        for (auto&& [key, value] : fields)
        {
            if (!first)
            {
                os << ",";
            }
            else
            {
                first = false;
            }
            os << '\"' << key << "\":";
            value->print(os);
        }
        os << '}';
    }

    JSON_Value* copy() const override
    {
        JSON_Object* tmp { new JSON_Object };
        for (auto&& [key, value] : fields)
        {
            tmp->fields[key] = value->copy();
        }
        return tmp;
    }
    
    template <typename F>
    void visit(F f)
    {
        for (auto&& [key, value] : fields)
        {
            f(key, *value);
        }

        for (auto&& [key, value] : fields)
        {
            if (auto p = dynamic_cast<JSON_Object*>(value); p)
            {
                p->visit(f);
            }
        }
    }

    JSON_Value*& lookup(std::string const& key)
    {
        return fields[key];
    }
    
private:

    std::unordered_map<std::string, JSON_Value*> fields{};
    
};

template <typename T, typename... Args>
JSON_Value* create(Args&&... args)
{
    return new T{std::forward<Args>(args)...};
}

void print_value(std::string const& key, JSON_Value const& value)
{
    std::cout << key << " = ";
    value.print(std::cout);
    std::cout << std::endl;
}

int main()
{
    JSON_Object obj1 {};

    obj1.lookup("null") = create<JSON_Null>();
    obj1.lookup("string") = create<JSON_Data<std::string>>("my string");
    obj1.lookup("number") = create<JSON_Data<double>>(3.14);

    JSON_Object* obj2 {new JSON_Object{}};
    obj2->lookup("test") = create<JSON_Data<std::string>>("another string");

    obj1.lookup("object") = std::move(obj2);
    
    JSON_Value* obj3 {obj1.copy()};
    
    obj1.lookup("child") = std::move(obj3);
    
    obj1.print(std::cout);
    std::cout << std::endl;

    obj1.visit(print_value);
}