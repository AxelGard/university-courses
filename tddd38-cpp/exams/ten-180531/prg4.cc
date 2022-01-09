#include <iostream>
#include <vector>
#include <string> 
#include <random>

using namespace std;

class Context {

    protected:
        int id;
        static int count;



    public:     
        Context(): id{count++} {}  
        Context(Context const&) = delete;
        
        virtual ~Context() = default;

        virtual void print(ostream& os) const {
            os << " Context [" << this->id << "]";
        }

        void reset(){
            this->id = this->count++;
        }

};

int Context::count {0};

class Named_Context : public Context {

    private: 
        string name;

    public: 
        Named_Context(string const& name)
            : name{name} //, Context{}
            { }

        void print(ostream& os) const override {
            os << "Named Context [" << this->id << "] \"" << this->name << "\"";
        } 

        void set(string name) {
            this->name = name;
        }
};


class Event {

    public:     
        Event() = default;
        Event(Event const&) = delete;
        virtual ~Event() = default;
        virtual void apply(Context & context) const = 0;

};

class Reset_Event : public Event {
    public: 

        using Event::Event;

        void apply(Context& context) const override {
            context.print(cout);
            cout << " reset to: ";
            context.reset(); // breaks const promis ?? 
            context.print(cout);
            cout << endl;
        }

};

class Rename_Event : public Event {

    private: 
        string name;

    public: 

        Rename_Event(string name)
            : name{name}, Event{}
            { }

        void apply(Context& context) const override {
            context.print(cout);
            cout << endl;
            if (auto named_context = dynamic_cast<Named_Context*>(&context); named_context){
                named_context->set(this->name); // breaks const promis 
            } 
            context.print(cout);
            cout << endl;

        }
};



void apply_events(Context & c, vector<const Event*> const v)
{
    for (auto & event : v)
        event->apply(c);
}

int main()
{
    Context c { };
    Named_Context nc { "My Context" };
    // add different events to the vector
    // at least one of each type
    vector<const Event*> v {new Reset_Event(), new Rename_Event{"this is a rename event now "}, new Reset_Event()};
    apply_events(c, v);
    apply_events(nc, v);

    for (auto event : v) 
        delete event;

}