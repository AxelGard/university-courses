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
#include <algorithm>
#include <map>

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

class Element {
    protected: 
        std::string tag{};
        
    public: 
        std::string id{};

        Element(std::string tag){
            this->tag = tag;
        }

        Element() {
            this->tag = "None";
        }
    
        virtual ~Element() = default;
        virtual void print(unsigned indent = 0)  = 0;

        void print_indent(unsigned indent) // could be done with for each but not rellt needed 
        {
            for (unsigned i{0}; i < indent; ++i)
            {
                std::cout << ' ';
            }
        }

        void print_tag()
        {
            std::cout << "<" << this->tag;
            if (this->id != "")
            {
                std::cout << " id='" << this->id << "'";
            }
            std::cout << ">";
        }

        virtual std::vector<Element*> query_if(bool(*predicate)(Element& e)){
            std::vector<Element*> result {};
            if (predicate(*this))
            {
                result.push_back(this);
            }
            return result;
        }

        virtual std::vector<Element*> query_tag(std::string const& tag){
            std::vector<Element*> result {};
            if (this->tag == tag)
            {
                result.push_back(this);
            }
            return result;
        }
        
};

class Content : public Element 
{
    private:
        std::string content{};

    public: 

        Content(std::string tag, std::string id, std::string content){
            this->tag = tag;
            this->id = id;
            this->content = content;
        }

        void print(unsigned indent = 0) override {
            std::cout << this->content << "</" << this->tag << ">";   
        }

};

class Container : public Element 
{
    private: 
        std::vector<Element*> children{};
    
    public:

        Container(std::string tag, std::string id = ""){
            this->tag = tag;
            this->id = id;
        }

        void add_child(Element& child){
            this->children.push_back(&child);
        }

        std::vector<Element*> get_children() const {
            return  this->children;
        }

        void print(unsigned indent = 0) override {
            this->print_indent(indent);
            this->print_tag();
            std::for_each(std::begin(this->children), 
                std::end(this->children), 
                [indent](Element* e){
                    e->print(indent); 
                });
        }

        std::vector<Element*> query_if(bool(*predicate)(Element& e)) override{
            std::vector<Element*> result {};
            if (predicate(*this))
            {
                result.push_back(this);
                for( Element* elm : this->children) {
                    std::vector<Element*> part{elm->query_if(predicate)};
                    for(Element* chi_elm : part)
                        result.push_back(chi_elm);
                }
            }
            return result;
        }

        std::vector<Element*> query_tag(std::string const& tag) override {
            std::vector<Element*> result {};
            if (this->tag == tag)
            {
                result.push_back(this);
                for( Element* elm : this->children) {
                    std::vector<Element*> part{elm->query_tag(tag)};
                    for(Element* chi_elm : part)
                        result.push_back(chi_elm);
                }
            }
            return result;
        }

};

class Standalone : public Element 
{
    private: 
        std::string content;

    public: 
        Standalone(std::string tag, std::string content = ""){
            this->tag = tag;
            this->content = content;
        }

        void print(unsigned indent = 0) override {
            std::cout << this->content << "</" << this->tag << ">";   
        }
};



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
            for (Element* child : container->get_children())
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

/*
class Hierarchy {
    private:
        std::vector<Element*> elements {};  

    public: 
        
        void insert(Element& element){
            bool unique = (std::find(this->elements.begin(), 
                                        this->elements.end(), element
                                        ) != this->elements.end());
            if(unique){
                this->elements.push_back(&element); 
                if (Container* container = dynamic_cast<Container*>(&element); container != nullptr){
                    for (Element* child : container->get_children())
                    {
                        insert(*child);
                    }
                }
            }
        }
        
        void insert(Container& element){
            bool unique = (std::find(this->elements.begin(), 
                                        this->elements.end(), element
                                        ) != this->elements.end());
            if(unique){
                this->elements.push_back(&element);                 
                for (Element* child : element.get_children())
                {
                    this->insert(*child);
                }
                
            }
        }
        
        Element* query_id(std::string const& id)
        {
            for (unsigned i{0}; i < this->elements.size(); ++i)
            {
                if (this->elements[i]->id == id)
                    return this->elements[i];
            }
            return nullptr;
        }

};
*/

bool only_content(Element& e)
{
    return dynamic_cast<Content*>(&e) != nullptr;
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

    Container body { "body" };
    Content header { "h3", "", "Title" };

    body.add_child(header);
    
    Container div1 { "div", "div-1" };

    body.add_child(div1);
    
    Container ul { "ul" };
    Content li1 { "li", "", "Element #1" };
    Content li2 { "li", "", "Element #2" };

    ul.add_child(li1);
    ul.add_child(li2);
    
    div1.add_child(ul);
    
    Container div2 {"div", "div-2" };

    body.add_child(div2);
    
    Content p { "p", "", "This is a paragraph" };

    div2.add_child(p);
    
    Standalone submit { "button", "submit-button" };

    body.add_child(submit);

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
    
    std::vector<Element*> contents { body.query_if(only_content) };
    for (Element* e : contents)
    {
        e->print();
        std::cout << std::endl;
    } 
    

   return 0; 
}