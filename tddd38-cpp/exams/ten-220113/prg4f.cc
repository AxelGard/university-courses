#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

class Element
{
public:
    Element(std::string const &name)
        : name{name}
    {
    }

    virtual ~Element() = default;

    virtual void print(std::ostream &os) const = 0;

protected:
    std::string const name{};
};

class Label : public Element
{
public:
    Label(std::string const &name, std::string const &text)
        : Element{name}, text{text}
    {
    }

    void print(std::ostream &os) const override
    {
        os << name << ": \"" << text << "\"\n";
    }

private:
    std::string const text{};
};

class Collection : public Element
{
public:
    using Element::Element;

    void insert(int item)
    {
        std::string str{std::to_string(item)};
        if (str.size() > column_width)
        {
            column_width = str.size();
        }
        items.push_back(item);
    }

protected:
    virtual void print_item(std::ostream &os, int item) const
    {
        os << std::setw(column_width) << item;
    }

    int column_width{0};
    std::vector<int> items{};
};

template <int width>
class Grid : public Collection
{
public:
    using Collection::Collection;

    void print(std::ostream &os) const
    {
        // Print header for the grid
        os << name << ":\n";
        for (unsigned i{0}; i < items.size(); ++i)
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

class List : public Grid<1>
{
public:
    using Grid::Grid;

protected:
    void print_item(std::ostream &os, int item) const override
    {
        os << " - [";
        Grid::print_item(os, item);
        os << "]";
    }
};

int main()
{
    std::vector<Element *> elements = {
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
            if (auto c = dynamic_cast<Collection *>(e))
            {
                c->insert(n);
            }
        }
    }

    // Print (and delete) each element
    for (auto e : elements)
    {
        e->print(std::cout);
        std::cout << std::endl;
        delete e;
    }
}