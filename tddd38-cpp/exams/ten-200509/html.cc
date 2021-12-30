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

// - You don't have to know how HTML works in order to make reasonable
//   changes. The focus is more on the features of the language.

// - It is ok if the behaviour of the code changes, as long as it
//   stays approximately the same. Use your own judgement.

// - There are a lot of comments in this file, read them.

#include <vector>
#include <string>
#include <iostream>

// This struct represents an element in the HTML hierarchy. Each
// element in the document hierarchy has a type, a tag and an
// (optional) id. The 'tag' is a string which represents what type of
// HTML element this is. For example, it could be "div", "table",
// "body" etc. The 'id' is either empty (meaning it has no id) or it
// contains an arbitrary string. Each HTML document has a textual
// representation which can be printed with the 'print' function that
// is defined further down.

// The 'type' variable determines which type of element this is. There
// are three types of elements:

// - Content   : An element that only contains a string. This string is
//               stored in 'content'

// - Container : Contains an arbitrary amount of elements called
//               children. These children are stored in 'children'

// - Standalone: An element that doesn't have any content nor any
//               children.

// The values of 'type' corresponds to these types like this:

// - type == 0: indicates Content

// - type == 1: indicates Container

// - type == 2: indicates Standalone

// Wishlist:

// - It would be a good idea to represent the 'type' in some other way.

// - would be nice if only the data relevant for the 'type' is stored in
//   that element

// - Maybe type == 0 (Content) should be able to store arbitrary data
//   and not just strings?

struct Element
{
    int type{};
    std::string tag{};
    std::string id{};
    std::string content{};
    std::vector<Element*> children{};
};

// Helper function that prints 'indent' number of spaces.
void print_indent(unsigned indent)
{
    for (unsigned i{0}; i < indent; ++i)
    {
        std::cout << ' ';
    }
}

// Helper function used to print the tag in the element 'e'.
// If the element has an id it will be printed as well.
void print_tag(Element const& e)
{
    std::cout << "<" << e.tag;
    if (e.id != "")
    {
        std::cout << " id='" << e.id << "'";
    }
    std::cout << ">";
}

// Print the textual representation of element 'e'. The behaviour
// varies depending on the type of element. The indent variable is
// used to make sure that the element is indented correctly.

// - Content   : prints the start tag followed by the content and then
//               the end tag. Assumes that this is just one line when
//               printed.

// - Container : print the start tag on its own line followed by
//               recursively printing each child with a deeper
//               indentation. It ends with the end tag of 'e'.

// - Standalone: print the start tag only. Assumes it is just one line
//               when printed.

// Wishlist:

// - Would be nice to split this function in such a way that we don't
//   need the enumeration of the type variable.

void print(Element const& e, unsigned indent = 0)
{
    print_indent(indent);
    print_tag(e);
    if (e.type == 0)
    {
        std::cout << e.content << "</" << e.tag << ">";
    }
    else if (e.type == 1)
    {
        std::cout << std::endl;
        for (unsigned i{0}; i < e.children.size(); ++i)
        {
            print(*e.children[i], indent + 4);
        }
        print_indent(indent);
        std::cout << "</" << e.tag << ">";
    }
    std::cout << std::endl;
}

// This function only works with Container type elements. It adds a
// new child to the container and returns a pointer to the newly added
// child.
Element* add_child(Element& e, Element& child)
{
    if (e.type == 1)
    {
        e.children.push_back(&child);
        return e.children.back();
    }
    return nullptr;
}

// This function will go through 'e' and each of its children
// recursively to find all elements which have the specified tag.

// Wishlist:

// - should use query_if instead.

std::vector<Element*> query_tag(Element& e, std::string const& tag)
{
    std::vector<Element*> result {};
    if (e.tag == tag)
    {
        result.push_back(&e);
    }

    // if the element is a Container, query their children as well
    if (e.type == 1)
    {
        for (unsigned i{0}; i < e.children.size(); ++i)
        {
            std::vector<Element*> part { query_tag(*e.children[i], tag) };
            for (unsigned j{0}; j < part.size(); ++j)
            {
                result.push_back(part[j]);
            }
        }
    }
    return result;
}

// This function will go through 'e' and each of its children
// recursively to find all elements for which the predicate function
// returns true.

// Wishlist:

// - it should be possible to pass in lambdas and function objets as well

// - maybe there should be two versions, one if 'e' is a Container and
//   one if its not.
std::vector<Element*> query_if(Element& e, bool(*predicate)(Element& e))
{
    std::vector<Element*> result {};
    if (predicate(e))
    {
        result.push_back(&e);
    }

    // if the element is a Container, query their children as well
    if (e.type == 1)
    {
        for (unsigned i{0}; i < e.children.size(); ++i)
        {
            std::vector<Element*> part { query_if(*e.children[i], predicate) };
            for (unsigned j{0}; j < part.size(); ++j)
            {
                result.push_back(part[j]);
            }
        }
    }
    return result;    
}

// This is a struct that keep track of every element in the document hierarchy.

// Wishlist:

// - the elements should only be possible to modify inside the
//   'insert' function.

struct Hierarchy
{
    std::vector<Element*> elements {};
};

// Insert an element into the document hierarchy. Each element is
// unique inside the hierarchy, meaning if you add it again nothing
// should happen.

// Wishlist:

// - It should be possible to reduce the number of times we visit each
//   element in the hierarchy somehow.

void insert(Hierarchy& hierarchy, Element& element)
{
    bool unique {true};
    for (unsigned i{0}; i < hierarchy.elements.size(); ++i)
    {
        if (element.id == hierarchy.elements[i]->id)
        {
            unique = false;
            break;
        }
    }

    if (unique)
    {
        hierarchy.elements.push_back(&element);

        // if this is a Container element, make sure that all of its
        // children are in the hierarchy as well.
        if (element.type == 1)
        {
            for (Element* child : element.children)
            {
                insert(hierarchy, *child);
            }
        }
    }
}

// This function retrieves the element in the hierarchy with the specified id.
// If no such element exists it will return nullptr.

// Wishlist:

// - would be nice if this didn't iterate through all elements (O(log n) should be possible)

Element* query_id(Hierarchy const& hierarchy, std::string const& id)
{
    for (unsigned i{0}; i < hierarchy.elements.size(); ++i)
    {
        if (hierarchy.elements[i]->id == id)
            return hierarchy.elements[i];
    }
    return nullptr;
}

// This is a helper function used for testing query_if
bool only_content(Element& e)
{
    return e.type == 0;
}

/*
  Expected output:

== body == 

<body>
    <h3>Title</h3>
    <div id='div-1'>
        <ul>
            <li>Element #1</li>
            <li>Element #2</li>
        </ul>
    </div>
    <div id='div-2'>
        <p>This is a paragraph</p>
    </div>
    <button id='submit-button'>
</body>

== divs == 

<div id='div-1'>
    <ul>
        <li>Element #1</li>
        <li>Element #2</li>
    </ul>
</div>

<div id='div-2'>
    <p>This is a paragraph</p>
</div>

== submit-button == 

<button id='submit-button'>

== content == 

<h3>Title</h3>

<li>Element #1</li>

<li>Element #2</li>

<p>This is a paragraph</p>  
  
 */
int main()
{

    // Create a HTML document
    
    Element body { 1, "body" };
    Element header { 0, "h3", "", "Title" };

    add_child(body, header);
    
    Element div1 { 1, "div", "div-1" };

    add_child(body, div1);
    
    Element ul { 1, "ul" };
    Element li1 { 0, "li", "", "Element #1" };
    Element li2 { 0, "li", "", "Element #2" };

    add_child(ul, li1);
    add_child(ul, li2);
    
    add_child(div1, ul);
    
    Element div2 { 1, "div", "div-2" };

    add_child(body, div2);
    
    Element p { 0, "p", "", "This is a paragraph" };

    add_child(div2, p);
    
    Element submit { 2, "button", "submit-button" };

    add_child(body, submit);

    Hierarchy hierarchy {};
    insert(hierarchy, body);

    // adding it again should do nothing
    insert(hierarchy, body);

    // Print the HTML document
    
    std::cout << "== body == \n" << std::endl;
    
    print(body);

    // Print all the divs 
    
    std::cout << "\n== divs == \n" << std::endl;
    
    std::vector<Element*> divs { query_tag(body, "div") };
    for (Element* div : divs)
    {
        print(*div);
        std::cout << std::endl;
    }

    // Print the submit-button
    
    std::cout << "== submit-button == \n" << std::endl;
    
    print(*query_id(hierarchy, "submit-button"));

    // Print all the elements that only contain content.
    
    std::cout << "\n== content == \n" << std::endl;
    
    std::vector<Element*> contents { query_if(body, only_content) };
    for (Element* e : contents)
    {
        print(*e);
        std::cout << std::endl;
    }    
}