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

// - It is ok if the behaviour of the code changes, as long as it
//   stays approximately the same. Use your own judgement.

// - If you feel like you have fulfilled the requirements for the exam
//   it is not necessary to modify every piece of code that might be
//   broken due to your changes. You can safely remove any function
//   that is not strictly necessary for your solution.

// - There are a lot of comments in this file, read them.

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// This class represents all the possible values that can be stored in
// a list, as well as the lists themselves. These types are:

// - "Integer" : Holds a plain old int value.

// - "Text" : Holds a std::string value.

// - "List" : Holds a list of values. This is represented by a
//            std::vector<Value*>.

// These values are printable with the 'print' function and copyable
// with the 'clone' function.

// Wishlist:

// - It would be a good idea to represent the 'type' in some other way.

// - It should be easy to add other types of values.

// - Would be nice if the list could be extended with arbitrary types
//   instead of forcing the user to choose between strings and
//   integers.

// - Can we store only the relevant data member for the corresponding
//   type somehow?

// - There are a lot of memory leaks right now, it would be a good if
//   all values gets deallocated when they are no longer needed.

struct Value
{
    std::string type { "Integer" };

    // Integer
    int integer {};

    // Text
    std::string text {};

    // List
    std::vector<Value*> list {};
};

// Constructors for the different value types.

// Wishlist:

// - Would be nice if these were actual constructors.

// - It would make it a lot easier if we made a general version of
//   these create functions that can be used to create any Value.

Value* create_integer(int integer)
{
    Value* result { new Value{"Integer"} };
    result->integer = integer;
    return result;
}

Value* create_text(std::string text)
{
    Value* result { new Value{"Text"} };
    result->text = text;
    return result;
}

Value* create_list(std::vector<Value*> list)
{
    Value* result { new Value{"List"} };
    result->list = list;
    return result;
}

// Destructor for the values.

// Wishlist:

// - This should be an actual destructor.

// - Would be nice if this actually deallocated values as well.

void destroy(Value* value)
{
    if (value->type == "List")
    {
        for (Value* element : value->list)
        {
            destroy(element);
        }
    }
}

// This function prints the contents of 'value' to the ostream
// 'os'. The behaviour varies based on what type of value it gets.

// Wishlist:

// - Can we redesign our code so the complete enumeration isn't
//   necessary?

// - Is there maybe some other way we can iterate through each element
//   in the List case?

void print(Value const* value, std::ostream& os)
{
    if (value->type == "Integer")
    {
        os << value->integer;
    }
    else if (value->type == "Text")
    {
        os << '\"' << value->text << '\"';
    }
    else if (value->type == "List")
    {
        os << "[ ";
        for (Value const* element : value->list)
        {
            print(element, os);
            os << ' ';
        }
        os << "]";
    }
}

// This function makes a deep copy of the supplied 'value' object.

// Wishlist:

// - Would be nice if we didn't have to check the type of the value.

// - Can we make a deep copy of the List case with algorithms?

Value* clone(Value const* value)
{
    if (value->type == "Integer")
    {
        return create_integer(value->integer);
    }
    else if (value->type == "Text")
    {
        return create_text(value->text);
    }
    else if (value->type == "List")
    {
        std::vector<Value*> list{};
        for (Value const* element : value->list)
        {
            list.push_back(clone(element));
        }
        return create_list(list);
    }
    return nullptr;
}

// This struct represents the various Operations that can be performed
// on List values. These are:

// - "Duplicate": Which adds a copy of the entire list to the end of
//                the list. No data is associated with this
//                operation.

// - "Filter": Stores a function pointer to a predicate function that
//             is used to filter out elements from the List. Namely,
//             it will throw away all elements for which the predicate
//             returns false.

// - "Replace Text": Keeps a list of substitutions that it should
//                   perform on Text elements in the list. This means
//                   it will only operate on Text values, all other
//                   values are kept unchanged. If any of the Text
//                   values match a key in the substitution it will
//                   replace it with the associated string.

// Assumption: These objects are never accessed through pointers.

// Wishlist:

// - Would be nice if we didn't have to store the type.

// - Maybe this can be separated into different classes?

// - Can the "Filter" case be extended such that it accepts any type
//   of callable object and not just functions?

// - Is there a more appropriate way to store the substitution rules?

// Predicate is a data type: it is a pointer to a function that
// returns bool and takes 'Value const*' as a parameter.
using Predicate = bool (*)(Value const*);
struct Operation
{
    std::string type { "Duplicate" };

    // Filter
    Predicate predicate {};

    // Replace Text
    std::vector<std::string> keys {};
    std::vector<std::string> values {};
};


// Helper function for finding what index a specific key has in the
// "Replace Text" operation.

// Wishlist:

// - Maybe this function could return an iterator instead?

// - It would be great if this function was only available in the
//   "Replace Text" case.
int get_index(Operation const& op, std::string key)
{
    if (op.type == "Replace Text")
    {
        for (unsigned i {0}; i < op.keys.size(); ++i)
        {
            if (op.keys[i] == key)
            {
                return i;
            }
        }
    }
    
    return -1;
}

// Helper function that adds a substitution rule for the "Replace
// Text" operation.

// Wishlist:

// - It would be great if this function was only available in the
//   "Replace Text" case.

void insert(Operation& op, std::string key, std::string value)
{
    if (op.type != "Replace Text")
        return;

    int index { get_index(op, key) };
    if (index == -1)
    {
        op.keys.push_back(key);
        op.values.push_back(value);
    }
    else
    {
        op.values[index] = value;
    }
}

// This function is called to perform the specified operation on a
// passed in List value.

// Note: This function should only deal with List values, all other
// types of values are ignored.

// Wishlist:

// - Some of these operations might be implemented with some type of
//   algorithm instead?

// - The "Filter" case leads to memory leaks, could this be fixed
//   somehow?

void apply(Operation& op, Value* value)
{
    // This function is only valid for List values
    if (value->type != "List")
        return;
    
    if (op.type == "Duplicate")
    {
        std::vector<Value*> result {value->list};
        for (unsigned i {0}; i < value->list.size(); ++i)
        {
            result.push_back(clone(value->list[i]));
        }
        value->list = result;
    }
    else if (op.type == "Filter")
    {
        // Make sure that there are no memory leaks here
        std::vector<Value*> result;
        for (Value* element : value->list)
        {
            if (op.predicate(element))
            {
                result.push_back(element);
            }
        }
        value->list = result;
    }
    else if (op.type == "Replace Text")
    {
        for (Value* element : value->list)
        {
            // This operation is only valid for Text values
            if (element->type == "Text")
            {
                int index { get_index(op, element->text) };
                if (index != -1)
                {
                    element->text = op.values[index];
                }
            }
        }
    }
}

//===========================
//======== Testcases ========
//===========================

bool pred0(Value const* value)
{
    return value->type == "Text";
}

bool pred1(Value const* value)
{
    if (value->type == "Integer")
    {
        return value->integer >= 0;
    }
    return true;
}

/* Expected output:

Testcase 1:
[ 0 "Hello" 1 "World!" ]
After filtering out only strings:
[ "Hello" "World!" ]
After replace text:
[ "Howdy" "World!" ]

Testcase 2:
[ 1 -5 2 "-3" ]
After duplication:
[ 1 -5 2 "-3" 1 -5 2 "-3" ]
After filtering out positive integers:
[ 1 2 "-3" 1 2 "-3" ]

Testcase 3:
[ "A" [ "B" "C" ] "D" ]
After duplication:
[ "A" [ "B" "C" ] "D" "A" [ "B" "C" ] "D" ]

*/

// ======= NOTICE =======

// It is OK to remove some of these testcases if you need to reduce
// the amout of code. But you still have to make sure that all aspects
// of your code works as you intended in some way. Maybe you could
// write smaller testcases?

// The idea of these testcases are to demonstrate how the Operation
// and Value structs interact with each other.

int main()
{
    {
        Value* list { create_list({ create_integer(0), create_text("Hello"),
                                    create_integer(1), create_text("World!")}) };

        Operation op1 { "Filter", pred0 };

        std::cout << "Testcase 1:" << std::endl;
        
        print(list, std::cout);
        std::cout << std::endl;

        std::cout << "After filtering out only strings:" << std::endl;
        
        apply(op1, list);
        print(list, std::cout);
        std::cout << std::endl;

        Operation op2 { "Replace Text" };
        insert(op2, "Hello", "Howdy");

        std::cout << "After replace text:" << std::endl;
        
        apply(op2, list);
        print(list, std::cout);
        std::cout << std::endl;
        
        destroy(list);
    }
    std::cout << std::endl;
    {
        Value* list { create_list({ create_integer(1), create_integer(-5),
                                    create_integer(2), create_text("-3") }) };

        Operation op1 { "Duplicate" };

        std::cout << "Testcase 2:" << std::endl;

        print(list, std::cout);
        std::cout << std::endl;

        std::cout << "After duplication:" << std::endl;

        apply(op1, list);
        print(list, std::cout);
        std::cout << std::endl;

        Operation op2 { "Filter", pred1 };

        std::cout << "After filtering out positive integers:" << std::endl;

        apply(op2, list);
        print(list, std::cout);
        std::cout << std::endl;

        destroy(list);
    }
    std::cout << std::endl;
    {
        Value* list { create_list({ create_text("A"),
                                    create_list({create_text("B"), create_text("C")}),
                                    create_text("D")}) };

        std::cout << "Testcase 3:" << std::endl;
        print(list, std::cout);
        std::cout << std::endl;

        Operation op1 { "Duplicate" };

        std::cout << "After duplication:" << std::endl;

        apply(op1, list);
        print(list, std::cout);
        std::cout << std::endl;
        
        destroy(list);
    }
}