// Suggestions and hints:

// - Read the 'main' function to understand how the code is used.

// - Don't worry too much about rewriting everything. Find a few
//   places where you can demonstrate your knowledge and only change
//   those places.

// - The wishlists are just hints/suggestions. You don't have to
//   follow them if you don't want to.

// - It is more important to understand how the code is used rather
//   than what it does.

// - To help you understand the code it might be helpful to make small
//   changes in the code and then see what effect it has on the
//   output.

// - You don't have to know how JSON works in order to make reasonable
//   changes. Just focus on what improvements can be made in the code.

// - It is ok if the behaviour of the code changes, as long as it
//   stays approximately the same. Use your own judgement.

// - There are a lot of comments in this file, read them.

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

struct JSON_Object;

class JSON_Value;

template <typename T, typename... Args>
JSON_Value* create(Args&&... args); // forward decleration 

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

        void print(std::ostream& os) const override {
            os << "null";
        }

        JSON_Value* copy() const override {
            return create<JSON_Null>();
        }

};


template <typename T>
class JSON_Data : public JSON_Value {
    private:
        T value;
    
    public: 
        JSON_Data(T const& value)
            : value{value}
            { }

        void print(std::ostream& os) const override
        {
            os << this->value;
        }

        JSON_Value* copy() const override {
            return create<JSON_Data<T>>(value);
        }

};

class JSON_Object : public JSON_Value
{
    private:    
        std::unordered_map<std::string, JSON_Value*> fields{};
    
    public:
        ~JSON_Object() override {
            for (auto&& [key, value] : this->fields){
                delete value;
            }
        }

        void print(std::ostream& os) const override {
            os << '{';
            bool first{true};
            for (auto&& [key, value] : this->fields){
                if (!first){
                    os << ',';
                }
                else {
                    first = false;
                }
                os << key << ":";
                value->print(os);
            }
        }

        JSON_Value* copy() const override
        {
            JSON_Object* tmp { new JSON_Object};
            for (auto&& [key, value] : this->fields){
                tmp->fields[key] = value->copy();
            }
            return tmp;
        }

        template<typename F>
        void visit(F f){
            for (auto&& [key, value] : this->fields){
                f(key, *value);
            }

            for (auto&& [key, value] : this->fields){
                auto p = dynamic_cast<JSON_Object*>(value);
                if (p) {
                    p->visit(f);
                }
            }
        }

        JSON_Value*& lookup(std::string const& key){
            return fields[key];
        }

};


template<typename T, typename... Args>
JSON_Value* create(Args&&... args)
{
    return new T{std::forward<Args>(args)...};
}

void print_value(std::string const& key, JSON_Value const& value){
    std::cout << key << " = ";
    value.print(std::cout);
    std::cout << std::endl;
}

/*

Expected output:

{"null":null,"string":"my string","number":3.140000,"object":{"test":"another string"},"child":{"null":null,"string":"my string","number":3.140000,"object":{"test":"another string"}}}
null = null
string = "my string"
number = 3.140000
object = {"test":"another string"}
child = {"null":null,"string":"my string","number":3.140000,"object":{"test":"another string"}}
test = "another string"
null = null
string = "my string"
number = 3.140000
object = {"test":"another string"}
test = "another string"

*/

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