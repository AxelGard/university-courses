#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;


class Element {

    protected:
        string const name;

    public:
        Element(string name)
            : name{name}
        { }

        virtual ~Element() = default;

        virtual void print(ostream& os) const = 0;

    
};



class Label : public Element {
    private: 
        string const text {}; 
    
    public: 
        Label(string const &name, string const &text)
            : Element{name}, text{text}
            { }

            void print(ostream &os) const override 
            {
                os << name << ": \"" << text << "\"\n";
            }
};

class Collection : public Element{
    protected:
        vector<int> items;
        int column_width = 0;
        virtual void print_item(ostream &os, int item) const {
            os << setw(column_width) << item;
        }

    public: 

        using Element::Element;

        void insert(int item) {
            string str {to_string(item)};
            if (str.size() > column_width)
                column_width = str.size();
            items.push_back(item);
        }
};

template <int width>
class Grid : public Collection
{
    public:

        using Collection::Collection;

        // Possible implementation for Grid::print
        // Will support printing of a grid even if the number of items is not
        // a multiple of the grid width.
        void print(std::ostream &os) const override
        {
            // Print header for the grid
            os << name << ":\n";
            for (int i{0}; i < items.size(); ++i)
            {
                // print each item
                print_item(os, items[i]);

                // If we have filled one row (or if this is the last item),
                // print a newline character
                if ((i + 1) % width == 0 || i + 1 == items.size())
                {
                    os << '\n';
                }
                else
                {
                    os << ' ';
                }
            }
        }
};

class List : public Grid<1> {
    public: 
        using Grid::Grid;

    protected:  
        void print_item(ostream &os, int item) const override {
            os << " - [";
            Grid::print_item(os, item);
            os << "]";
        }
}; 



/* Expected output:

My Text: "This is a text"

My Grid (3):
  1  12 123
456   4  45
 78 789   7

My List:
 - [  1]
 - [ 12]
 - [123]
 - [456]
 - [  4]
 - [ 45]
 - [ 78]
 - [789]
 - [  7]

My Grid (2):
  1  12
123 456
  4  45
 78 789
  7

*/

int main()
{
    std::vector<Element*> elements = {
        new Label{"My Label", "This is a text"},
        new Grid<3>{"My Grid (3)"},
        new List{"My List"},
        new Grid<2>{"My Grid (2)"}};
    std::vector<int> data{1, 12, 123, 456, 4, 45, 78, 789, 7};

    // Populate each collection with 'data'
    for (int n : data)
    {
        for (auto e : elements)
        {
            // If e is of type 'Collection':
            // call insert(n) on it
            if (auto p = dynamic_cast<Collection*>(e))
                p->insert(n);
        }
    }

    // Print each element
    for (auto e : elements)
    {
        // call print(std::cout) on e
        e->print(cout);
        std::cout << std::endl;
        delete e; // rm from heap 
    }


}