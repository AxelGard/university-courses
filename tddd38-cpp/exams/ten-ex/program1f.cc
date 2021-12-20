#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;



class Serilizeble{
    public:
        virtual string serialize() const =0;
        virtual bool deserialize(string const& str)=0;
};

class Printable {
    public:
        virtual void print(ostream& os) const =0;
};

class Message : public Printable {
    public:
        Message(string const& msg)
            : msg{msg}
        { }

        void print(ostream& os) const override 
        {
            os << msg;
        }
    private:
        string msg;
};

class Integer : public Printable, public Serilizeble
{
    public:
        Integer(int i)
            : data{i}
            { }
    
    string serialize() const override
    {
        ostringstream oss{};
        oss << data;
        return oss.str();
    }

    bool deserialize(string const& str) override
    {
        istringstream iss{str};
        return static_cast<bool>(iss >> data);
    }

    void print(ostream& os) const override
    {
        os << data;
    }

    private:
        int data;
};

vector<string> serialize(vector<Printable*> const& v)
{
  vector<string> result{};
  for (Printable* obj : v)
  {
    if (auto p = dynamic_cast<Serilizeble*>(obj))
    {
      result.push_back(p->serialize());
    }
  }
  return result;
}

void print(ostream& os, vector<Printable*> const& v)
{
  for (Printable* obj : v)
  {
    /* Call print on obj with os as parameter */
    obj->print(os);
    os << endl;
  }
}

int main()
{
  vector<Printable *> v = {
    new Message{"Hello there"},
    new Integer{0},
    new Message{"This is a message"},
    new Integer{100053},
    new Integer{-5}
  };

  {
    vector<string> result {serialize(v)};
    assert((result == vector<string>{"0", "100053", "-5"}));
  }
  
  {
    ostringstream oss{};
    print(oss, v);
    assert(oss.str() == "Hello there\n0\nThis is a message\n100053\n-5\n");
  }

  {
    Integer i{0};
    assert(i.deserialize("15"));
    assert(i.serialize() == "15");
  }
  
}