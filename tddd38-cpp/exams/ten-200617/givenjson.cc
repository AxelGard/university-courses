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

struct JSON_Object;

// A struct representing the value of a field in a JSON object. Can be
// different types. The variable 'type' determines which type it is:

// - "null"   : Represents an empty field. It has no data.

// - "string" : represents a string value. Stores the string value in
//              'value'.

// - "number" : represents a numerical value. Stores the numerical
//              value as its string representation in the 'value'
//              variable.

// - "object" : represents a JSON object. Stores a pointer to the
//              actual JSON object in 'object'.

// Wishlist:

// - It would be a good idea to represent the 'type' in some other way.

// - would be nice if only the data relevant for the 'type' is stored in
//   that object

// - Maybe we can represent the value of the "number" type as a double instead?

// - It should be easy to introduce other types, such as bool, int,
//   etc. if we let one 'type' represent 'arbitrary data
//   types'. This would probably allow us to simplify "string" and
//   "number" since they are 'arbitrary data types'.

// - In the future it should be easy to add lists as a potential JSON_Value.
//   You don't have to add that, but make sure that I can add it later.

// - This object might cause memory leaks. That should probably be fixed.

struct JSON_Value
{
    std::string  type   {"null"};
    JSON_Object* object {};
    std::string  value  {};
};

// Function used to create a JSON Value that is null
JSON_Value* create_null()
{
    return new JSON_Value{};
}

// Function used to create a JSON Value that contains a string
JSON_Value* create_string(std::string const& value)
{
    return new JSON_Value{"string", nullptr, value};
}

// Function used to create a JSON Value that contains a number
JSON_Value* create_number(double value)
{
    return new JSON_Value{"number", nullptr, std::to_string(value)};
}

// Function used to create a JSON Value that contains an object
JSON_Value* create_object(JSON_Object* object)
{
    return new JSON_Value{"object", object};
}

// forward declaration since 'dump' uses it. Look further down for the
// definition.
void print(JSON_Value* value, std::ostream& os);

// forward declaration since the other 'copy' uses it. Look further
// down for the definition.
JSON_Value* copy(JSON_Value* value);


// This struct represents a JSON object. A JSON object contains
// key-value pairs, where the keys are strings and the values are
// represented as pointers to JSON_Value objects.

// Each key in 'keys' corresponds to a value in 'values'.  A key and a
// value is associated with each other if they have the same index in
// their respective vectors.

// Wishlist:

// - maybe we can store the keys and the values in one data
//   structure?

// - It would be nice if the user was only able to modify this object
//   with the functions defined below. I.e. we don't want the user to
//   have full access to 'keys' and 'values'.

// - This object might cause memory leaks. That should probably be fixed.

struct JSON_Object
{
    std::vector<std::string> keys   {};
    std::vector<JSON_Value*> values {};
};

// This functions finds the value associated with 'key' in 'object'
// and returns it. If that key was not found, it inserts that key and
// associates it with 'nullptr'.

// Since the value is returned by reference it is also possible to
// change which JSON_Value object it points to. For example:

// lookup(obj, "test") = new JSON_Value{};

// It can also be used to retrieve the value:

// print(lookup(obj, "test"), std::cout);

// Wishlist:

// - This is quite slow, can we speed it up somehow?

JSON_Value*& lookup(JSON_Object& object, std::string const& key)
{
    for (int i{0}; i < object.keys.size(); ++i)
    {
        if (object.keys[i] == key)
        {
            return object.values[i];
        }
    }

    // 'key' wasn't found. Insert it:
    
    object.keys.push_back(key);
    object.values.push_back(nullptr);
    return object.values.back();
}

// Print the content of 'object' in compact form. Will recursively
// print every value stored in the object.
void dump(JSON_Object const& object, std::ostream& os)
{
    os << '{';
    for (int i{0}; i < object.keys.size(); ++i)
    {
        if (i != 0)
        {
            os << ",";
        }
        os << '\"' << object.keys[i] << "\":";
        print(object.values[i], os);
    }
    os << '}';
}

// Make a deep copy of a JSON_Object and return a pointer to the copy.
JSON_Object* copy(JSON_Object* object)
{
    JSON_Object* result {new JSON_Object{}};
    result->keys = object->keys;
    for (int i{0}; i < object->values.size(); ++i)
    {
        result->values.push_back(copy(object->values[i]));
    }
    return result;
}

// Print the value of 'value'. The behaviour varies depending on the
// type.

// - null : Print the string "null"

// - string : Print the value as a string, surrounded by "

// - number : Print the numerical string representation

// - object : dump the entire object with the help of the 'dump'
//            function.

// Wishlist:

// - Would be nice to split this function in such a way that we don't
//   need the enumeration of the type variable.

void print(JSON_Value* value, std::ostream& os)
{
    if (value->type == "null")
    {
        os << "null";
    }
    else if (value->type == "string")
    {
        os << '\"' << value->value << '\"';
    }
    else if (value->type == "number")
    {
        os << value->value;
    }
    else if (value->type == "object")
    {
        dump(*value->object, os);
    }
}

// This function makes a deep copy of a JSON_Value object and return
// the new object as a pointer.

// Wishlist:

// - Would be nice to split this function in such a way that we don't
//   need the enumeration of the type variable.

JSON_Value* copy(JSON_Value* value)
{
    JSON_Value* result {new JSON_Value{value->type}};
    if (value->type == "string")
    {
        result->value = value->value;
    }
    else if (value->type == "number")
    {
        result->value = value->value;
    }
    else if (value->type == "object")
    {
        result->object = copy(value->object);
    }
    return result;
}

// This function goes through each value stored in 'object' and apply
// the function 'f' on it. If any objects are found they will be
// recursively visited as well (but the recursion only occurs after
// all values directly stored in 'object' have been visited).

// Wishlist:

// - it should be possible to pass in lambdas and function objets as well

void visit(JSON_Object* object, void (*f)(std::string const& key, JSON_Value*))
{
    for (int i {0}; i < object->keys.size(); ++i)
    {
        f(object->keys[i], object->values[i]);
    }

    for (int i {0}; i < object->keys.size(); ++i)
    {
        if (object->values[i]->type == "object")
        {
            visit(object->values[i]->object, f);
        }
    }
}


// This is a helper function used for testing 'visit'
void print_value(std::string const& key, JSON_Value* value)
{
    std::cout << key << " = ";
    print(value, std::cout);
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

    lookup(obj1, "null") = create_null();
    lookup(obj1, "string") = create_string("my string");
    lookup(obj1, "number") = create_number(3.14);

    JSON_Object* obj2 {new JSON_Object{}};
    lookup(*obj2, "test") = create_string("another string");

    lookup(obj1, "object") = create_object(obj2);
    
    JSON_Object* obj3 {copy(&obj1)};
    
    lookup(obj1, "child") = create_object(obj3);
    
    dump(obj1, std::cout);
    std::cout << std::endl;

    visit(&obj1, print_value);
}