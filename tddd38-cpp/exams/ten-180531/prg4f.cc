#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Context
{
public:

    Context() : id{count++} {}

    Context(Context const&) = delete;
    
    virtual ~Context() = default;

    virtual void print(ostream & os) const
    {
        os << "Context [" << id << ']';
    }

    void reset() { id = count++; }
    
private:
    static int count;
    int id;
};

// You need to define (initialize) static data members
// outside of the class since only one definition is
// allowed by the compiler.
int Context::count {0};

class Named_Context : public Context
{
public:

    Named_Context(string const & name) : name{name} {}
    
    void print(ostream & os) const override
    {
        os << "Named ";
        Context::print(os);
        os << ": \"" << name << '"';
    }

    void set(string const & name) { this->name = name; }

private:
    string name;
    
};

class Event
{

public:

    Event() = default;
    Event(Event const &) = delete;
    virtual ~Event() = default;

    virtual void apply(Context & context) const = 0;
    
};

class Reset_Event : public Event
{

public:

    void apply(Context & context) const override
    {
        context.print(cout);
        cout << " reset to: ";
        context.reset();
        context.print(cout);
        cout << endl;
    }
    
};

class Rename_Event : public Event
{

public:

    Rename_Event(string const & name) : name{name} {}
    
    void apply(Context & context) const override
    {
        context.print(cout);
        cout << " renamed to: ";
        if (auto p = dynamic_cast<Named_Context*>(&context); p)
            p->set(name);
        context.print(cout);
        cout << endl;
    }

private:

    string name;
    
};

void apply_events(Context & c, vector<Event const*> const & v)
{
    for (Event const * event : v)
        event->apply(c);
}

int main()
{
    Context c { };
    Named_Context nc { "My Context" };
    vector<Event const*> v { new Reset_Event(),
                             new Rename_Event("Your Context")};
    apply_events(c, v);
    apply_events(nc, v);

    for (auto event : v)
    {
        delete event;
    }
}