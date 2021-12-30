#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

class Element;

template <typename Predicate>
std::vector<Element*> query_if(Element& e, Predicate p);

class Element
{
public:

    Element(std::string const& tag, std::string const& id = "")
        : tag{tag}, id{id}
    { }
    
    virtual ~Element() = default;
    
    virtual void print(unsigned indent = 0) const
    {
        print_indent(indent);
        print_tag();
    }
    
    std::vector<Element*> query_tag(std::string const& tag)
    {
        return query_if(*this, [&tag](Element& e) { return e.tag == tag; });
    }
    
protected:
    
    void print_tag() const
    {
        std::cout << "<" << tag;
        if (id != "")
        {
            std::cout << " id='" << id << "'";
        }
        std::cout << ">";
    }


    // Helper function that prints 'indent' number of spaces.
    static void print_indent(unsigned indent) 
    {
        for (unsigned i{0}; i < indent; ++i)
        {
            std::cout << ' ';
        }
    }

    std::string tag;
    std::string id;

    friend class Hierarchy;
    
};

class Content : public Element
{
public:

    Content(std::string const& tag, std::string const& content, std::string const& id = "")
        : Element{tag, id}, content{content}
    { }
    
    void print(unsigned indent = 0) const override
    {
        Element::print(indent);
        std::cout << content << "</" << tag << ">"
                  << std::endl;
    }
    
private:

    std::string content{};
    
};

class Container : public Element
{
public:

    using Element::Element;
    
    void print(unsigned indent = 0) const override
    {
        Element::print(indent);
        std::cout << std::endl;
        for (Element* child : children)
        {
            child->print(indent + 4);
        }
        print_indent(indent);
        std::cout << "</" << tag << ">"
                  << std::endl;
    }

    Element* add_child(Element& child)
    {
        children.push_back(&child);
        return &child;
    }

    template <typename Predicate>
    friend std::vector<Element*> query_if(Element& e, Predicate p);

    friend class Hierarchy;
    
private:

    std::vector<Element*> children;
    
};

class Standalone : public Element
{
public:

    using Element::Element;
    
    void print(unsigned indent = 0) const override
    {
        Element::print(indent);
        std::cout << std::endl;
    }
    
};

template <typename Predicate>
std::vector<Element*> query_if(Element& e, Predicate p)
{
    std::vector<Element*> result {};
    if (p(e))
    {
        result.push_back(&e);
    }

    if (Container* container = dynamic_cast<Container*>(&e); container != nullptr)
    {
        for (Element* child : container->children)
        {
            std::vector<Element*> part { query_if(*child, p) };
            std::copy(std::begin(part), std::end(part), std::back_inserter(result));
        }
    }
    
    return result;
}

class Hierarchy
{
public:

    void insert(Element& element)
    {
        if (element.id != "")
        {
            lookup[element.id] = &element;
        }
        if (Container* container = dynamic_cast<Container*>(&element); container != nullptr)
        {
            for (Element* child : container->children)
            {
                insert(*child);
            }
        }
    }

    Element* query_id(std::string const& id) const
    {
        auto it {lookup.find(id)};
        if (it == std::end(lookup))
        {
            return nullptr;
        }
        return it->second;
    }
    
private:

    std::map<std::string, Element*> lookup {};
};

bool only_content(Element& e)
{
    return dynamic_cast<Content*>(&e) != nullptr;
}

int main()
{

    // Create a HTML document
    
    Container body { "body" };
    Content header { "h3", "Title" };

    body.add_child(header);
    
    Container div1 { "div", "div-1" };

    body.add_child(div1);
    
    Container ul { "ul" };
    Content li1 { "li", "Element #1" };
    Content li2 { "li", "Element #2" };

    ul.add_child(li1);
    ul.add_child(li2);
    
    div1.add_child(ul);
    
    Container div2 { "div", "div-2" };

    body.add_child(div2);
    
    Content p { "p", "This is a paragraph" };

    div2.add_child(p);
    
    Standalone submit { "button", "submit-button" };

    div2.add_child(submit);

    Hierarchy hierarchy {};
    hierarchy.insert(body);

    // adding it again should do nothing
    hierarchy.insert(body);

    // Print the HTML document
    
    std::cout << "== body == \n" << std::endl;
    
    body.print();

    // Print all the divs 
    
    std::cout << "\n== divs == \n" << std::endl;
    
    std::vector<Element*> divs { body.query_tag("div") };
    for (Element* div : divs)
    {
        div->print();
        std::cout << std::endl;
    }

    // Print the submit-button
    
    std::cout << "== submit-button == \n" << std::endl;
    
    hierarchy.query_id("submit-button")->print();

    // Print all the elements that only contain content.
    
    std::cout << "\n== content == \n" << std::endl;
    
    std::vector<Element*> contents { query_if(body, only_content) };
    for (Element* e : contents)
    {
        e->print();
        std::cout << std::endl;
    }    
}